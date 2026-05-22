
-- ========================================================================
-- do not change anything in this module header
-- ========================================================================

module HW6 where 


-- The only bits of the standard Haskell library that will be available to you
-- are the things you see imported here:

import FSA hiding ((>=>))
import CFG
import Algebra
import Prelude
  ( -- the basic types that we've seen for integers, booleans, characters, and strings
    Int, Bool(True, False), Char, String
    -- the type constructor Maybe (and its data constructors, Just and Nothing)
  , Maybe(..)
    -- some basic arithmetic operations and relations:
  , (+), (-), (*), (<), (>), (==), (>=), (<=), maximum, minimum, max, min
    -- some basic boolean operations:
  , (&&), (||), not
    -- some list inspection and processing functions
  , take, drop, or, and, any, all, map, filter, elem, (++), (.), words
    -- and some classes for showing and comparing things (don't worry about these)
  , Show, Eq, undefined
  )
  
-- ========================================================================
-- make your changes below here:
-- ========================================================================


-- Rule 1: P -> dd
-- Rule 2: P -> pp
-- Rule 3: P -> d P d
-- Rule 4: P -> p P p

-- uncomment these next lines and define a type for your nonterminals:
data PGCats = Start     -- ^ start symbol generating even-length palindromes
  | Dsym
  | Psym
  | Dwrap
  | Pwrap
  deriving (Show, Eq)
pg:: CFG PGCats
pg = CFGWith [Start] rules
  where
    rules =
      [ Dsym  :- "d"
      , Psym  :- "p"
      , Start :< (Dsym, Dsym)
      , Start :< (Psym, Psym)
      , Start :< (Dsym, Dwrap)
      , Dwrap :< (Start, Dsym)
      , Start :< (Psym, Pwrap)
      , Pwrap :< (Start, Psym)
      ]

ftc :: FSA -> CFG Int
ftc (FSAWith states symbols starts stops delta) =
  CFGWith starts (termRules ++ epsRules ++ binRules)
  where
    maxState    = maximum states
    offset      = maxState + 1
    symList     = symbols
    symbolIndex :: Char -> Int
    symbolIndex c = go symList 0
      where go (x:xs) n = if x == c then n else go xs (n + 1)
    termCat :: Char -> Int
    termCat c = offset + symbolIndex c
    termRules = [ termCat c :- return c | c <- symbols ]
    epsRules  = [ f :- "" | f <- stops ]
    binRules  = [ p :< (termCat c, q) | (p, c, q) <- delta ]

s1 :: String
s1 = "this book every student I know read stinks"

s1Parsed :: [LBT Cats]
s1Parsed = parseLBT eng (words s1)

phrase1 :: String
phrase1 = "while Sam watched the baby"

phrase1Parsed :: [LBT Cats]
phrase1Parsed = parseLBT eng (words phrase1)

phrase2 :: String
phrase2 = "while Sam watched the baby cried"

phrase2Parsed :: [LBT Cats]
phrase2Parsed = parseLBT eng (words phrase2)

-- put your thoughts
-- in the comments
-- here
-- In the parse of "while Sam watched the baby", "watched" is taken as a transitive verb, so "the baby" is grouped as its object.
-- In the parse of "while Sam watched the baby cried", the parser first groups "the baby" as the object of "watched", then must reanalyze it as the subject of "cried", illustrating the garden-path effect.

chart :: [((Int,Int),[Cats])]
chart = [((0,1),[W]),((0,2),[]),((0,3),[WP]),((0,4),[]),((0,5),[WP]),((0,6),[S]),
                    ((1,2),[DP]),((1,3),[RC,S]),((1,4),[]),((1,5),[S]),((1,6),[]),
                               ((2,3),[VT,VP]),((2,4),[]),((2,5),[VP]),((2,6),[]),
                                           ((3,4),[D]),((3,5),[DP]),((3,6),[S]),
                                                      ((4,5),[NP]),((4,6),[]),
                                                                 ((5,6),[VP])]
                                                                



parseMT :: (Eq nt, Monoid v) => [(Rule nt, v)] -> [String] -> [(LBT nt, v)]
parseMT mcfg [w] =
  [ (Leaf c w, m)
  | (c :- v, m) <- mcfg
  , v == w
  ]

parseMT mcfg ws =
  [ (Branch c lt rt, m_rule <*> m_l <*> m_r)
  | (left, right)           <- splits ws
  , (lt,   m_l)             <- parseMT mcfg left
  , (rt,   m_r)             <- parseMT mcfg right
  , (c :< (lcat, rcat), m_rule) <- mcfg
  , lcat == root lt
  , rcat == root rt
  ]
