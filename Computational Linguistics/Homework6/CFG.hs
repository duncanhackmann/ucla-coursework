
module CFG where

import Prelude hiding (Monoid(..), Semigroup(..), (<*>))
import Data.Tree
import Algebra

-- CFGs
-- ====================================================

data Rule nt
  = nt :- String
  | nt :< (nt, nt)
  deriving (Eq, Show, Ord)

data CFG nt = CFGWith [nt] [Rule nt]

type Phrase = [String]

-- Labeled Binary Trees
-- ====================================================

data LBT nt
  = Leaf nt String
  | Branch nt (LBT nt) (LBT nt)
  deriving (Eq, Show, Ord)

root :: LBT nt -> nt
root (Leaf c _)     = c
root (Branch c _ _) = c


-- Pretty-printed trees
-- ====================================================

toTree :: Show nt => LBT nt -> Tree String
toTree (Leaf   c x)   = Node (show c) [Node (show x) []]
toTree (Branch c l r) = Node (show c) [toTree l, toTree r]

displayForest :: Show nt => [LBT nt] -> IO ()
displayForest = putStrLn . drawForest . map toTree


-- Some small English grammars
-- ====================================================

data Cats = S | D | DP | NP | VT | VP | P | PP | WP | W | RC | C
  deriving (Eq, Show, Ord)


eng :: [Rule Cats]
eng =
  [ S  :< (DP, VP)
  , S  :< (WP, S )     -- preposed while-phrases
  , WP :< (W , S)      -- making a while-phrase
  , VP :< (VT, DP)
  , DP :< (D , NP)
  , NP :< (NP, PP)
  , NP :< (NP, RC)     -- relative clause modification of an NP
  , PP :< (P , DP)
  , VP :< (VP, PP)
  , RC :< (C , VP)     -- subject-gap rel clauses require "that"
  , RC :< (DP, VT)     -- object-gap rel clauses without "that"
  , DP :- "Sam"
  , DP :- "I"
  , VT :- "wrote"
  , VT :- "read"
  , VT :- "know"
  , VT :- "watched"    -- "watched" is transitive
  , VP :- "watched"    -- "watched" is also intransitive
  , VP :- "cried"
  , VP :- "awoke"
  , VP :- "stinks"
  , D  :- "this"
  , D  :- "the"
  , D  :- "every"
  , NP :- "book"
  , NP :- "baby"
  , NP :- "student"
  , P  :- "with"
  , C  :- "that"
  , W  :- "while"
  ]

cfg14 :: [Rule Cats]
cfg14 =
  [ S  :< (DP,VP)
  , VP :< (VT,NP)
  , NP :< (NP,PP)
  , PP :< (P ,NP)
  , VP :< (VP,PP)
  , DP :- "Mary"
  , DP :- "Sam"
  , NP :- "drones"
  , NP :- "spies" -- "spies" is a noun
  , NP :- "watches"  -- "watches" is a noun
  , VP :- "spies" -- "spies" is also a transitive verb
  , VT :- "watches"  -- "watches" is also a transitive verb
  , P  :- "with"
  ]


type PCFG nt = [(Rule nt, Prob)]

-- A probabilistic version of cfg14
pcfg14 :: PCFG Cats
pcfg14 =
  [ (S  :< (DP,VP)  , 1.0)
  , (VP :< (VT,NP)  , 0.4)
  , (NP :< (NP,PP)  , 0.4)
  , (PP :< (P ,NP)  , 1.0)
  , (VP :< (VP,PP)  , 0.3)
  , (DP :- "Mary"   , 0.4)
  , (DP :- "Sam"    , 0.6)
  , (NP :- "drones" , 0.1)
  , (NP :- "watches", 0.3)
  , (NP :- "spies"  , 0.2)
  , (VP :- "spies"  , 0.3)
  , (VT :- "watches", 1.0)
  , (P  :- "with"   , 1.0)
  ]


-- PARSING
-- ====================================================

splits :: [a] -> [([a], [a])]
splits u = [splitAt i u | i <- [1..length u - 1]]

(>=>) :: (a -> [b]) -> (b -> [c]) -> a -> [c]
f >=> g = \a -> [c | b <- f a, c <- g b]


-- Naive parsing to categories
-- ====================================================

stepLex :: [Rule nt] -> String -> [nt]
stepLex cfg w =
  [ c | (c :- v) <- cfg, w==v ]

stepBin :: Eq nt => [Rule nt] -> [nt] -> [nt] -> [nt]
stepBin cfg left right =
  [ c | lcat <- left, rcat <- right, (c :< (l, r)) <- cfg
      , l==lcat, r==rcat ]

combine :: Eq nt => [Rule nt] -> (Phrase, Phrase) -> [nt]
combine cfg (left, right) = stepBin cfg lcats rcats
  where lcats = parse cfg left
        rcats = parse cfg right

parse :: Eq nt => [Rule nt] -> Phrase -> [nt]
parse cfg [w] = stepLex cfg w
parse cfg phr = go phr
  where go = splits >=> combine cfg


-- Naive parsing to trees
-- ====================================================

stepLexLBT :: [Rule nt] -> String -> [LBT nt]
stepLexLBT cfg w =
  [ Leaf c w | (c :- v) <- cfg, w==v ]

stepBinLBT :: Eq nt => [Rule nt] -> [LBT nt] -> [LBT nt] -> [LBT nt]
stepBinLBT cfg left right =
  [ Branch c ltree rtree | ltree <- left, rtree <- right
                         , (c :< (l,r)) <- cfg, l == root ltree, r == root rtree ]

combineLBT :: Eq nt => [Rule nt] -> (Phrase, Phrase) -> [LBT nt]
combineLBT cfg (left, right) = stepBinLBT cfg lts rts
  where lts = parseLBT cfg left  -- parse the left daughter
        rts = parseLBT cfg right -- parse the right daughter

parseLBT :: Eq nt => [Rule nt] -> Phrase -> [LBT nt]
parseLBT cfg [w] = stepLexLBT cfg w
parseLBT cfg phr = go phr
  where go = splits >=> combineLBT cfg


-- Naive parsing to monoid-weighted categories
-- ====================================================

type CFT nt v = [(Rule nt, v)]

stepLexM :: Monoid v => CFT nt v -> String -> [(nt, v)]
stepLexM cfg w =
  [ (c, m) | (c :- v, m) <- cfg, w==v ]

stepBinM :: (Eq nt, Monoid v) => CFT nt v -> [(nt, v)] -> [(nt, v)] -> [(nt, v)]
stepBinM cfg left right =
  [ (c, m<*>n<*>o) | (lcat, n) <- left, (rcat, o) <- right
                 , (c :< (l,r), m) <- cfg, l==lcat, r==rcat ]

combineM :: (Eq nt, Monoid v) => CFT nt v -> (Phrase, Phrase) -> [(nt, v)]
combineM cfg (left, right) = stepBinM cfg lts rts -- combine the results
  where lts = parseM cfg left
        rts = parseM cfg right

parseM :: (Eq nt, Monoid v) => CFT nt v -> Phrase -> [(nt, v)]
parseM cfg [w] = stepLexM cfg w
parseM cfg phr = go phr
  where go = splits >=> combineM cfg


-- CKY parsing to categories
-- ====================================================

-- an append operator, just for convenience
(+:) :: [a] -> a -> [a]
xs +: x = xs ++ [x]

-- a lookup function for keys that are known to be in a dictionary
lookup' :: Eq k => k -> [(k,v)] -> v
lookup' key ((k,v):tail) = if key == k then v else lookup' key tail

-- all the suffixes of a list
tails :: [a] -> [[a]]
tails []     = []
tails (x:xs) = tails xs +: (x:xs)

-- all the prefixes of a list
inits :: [a] -> [[a]]
inits = go [] [] where
  go cur res []     = res
  go cur res (x:xs) = let next = cur +: x in go next (res +: next) xs

-- all the suffixes of all the prefixes
spans :: [a] -> [[a]]
spans = inits >=> tails


-- parse every sub-span of the sentence exactly once, using the results of
-- previous parses instead of re-parsing
-- note that the chart is an "accumulating parameter" in the recursion, similar
-- to the `enumerate` and `merge` functions we've written in class, or
-- the `inits` function above

cky :: Eq nt => [Rule nt] -> Phrase -> [nt]
cky cfg xs = let ((_, cats):_) = go (spans xs) [] in cats
 where
  go []         chart = chart
  go ([w]:phrs) chart = go phrs (([w],r):chart)
    where
      r = [c | (c :- v) <- cfg, v==w]
  go (phr:phrs) chart = go phrs ((phr,r):chart)
    where
      r = [ c | (left,right) <- splits phr
              , lcat <- lookup' left  chart
              , rcat <- lookup' right chart
              , (c :< (l,r)) <- cfg, l==lcat, r==rcat ]

ckyM :: (Eq nt, Monoid v) => CFT nt v -> Phrase -> [(nt, v)]
ckyM mcfg xs = let ((_, cats):_) = go (spans xs) [] in cats
 where
  go []         chart = chart
  go ([w]:phrs) chart = go phrs (([w], r):chart)
    where
      r = [(c, m) | (c :- v, m) <- mcfg, v==w]
  go (phr:phrs) chart = go phrs ((phr, r):chart)
    where
      r = [ (c, m<*>n<*>o) | (left,right) <- splits phr
                           , (lcat, n) <- lookup' left  chart
                           , (rcat, o) <- lookup' right chart
                           , (c :< (l,r), m) <- mcfg, l==lcat, r==rcat ]

cky' :: (Eq nt, Semiring v) => CFT nt v -> Phrase -> [(nt, v)]
cky' scfg xs = let ((_, cats):_) = go (spans xs) [] in cats
 where
  go []         chart = chart
  go ([w]:phrs) chart = go phrs (([w], r):chart)
    where
      r = [(c, m) | (c :- v, m) <- scfg, v==w]
  go (phr:phrs) chart = go phrs ((phr, compress r):chart)
    where
      r = [ (c, m<*>n<*>o) | (left,right) <- splits phr
                           , (lcat, n) <- lookup' left  chart
                           , (rcat, o) <- lookup' right chart
                           , (c :< (l,r), m) <- scfg, l==lcat, r==rcat ]
