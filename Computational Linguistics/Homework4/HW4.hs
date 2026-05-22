
-- ========================================================================
-- do not change anything in this module header
-- ========================================================================

module HW4 where 


-- The only bits of Haskell that will be available to you in this assignment
-- are the things you see imported here at the top. In particular, the only
-- pre-defined Haskell functions and types that you'll be able to use are the
-- ones you see in `import Prelude` block.

import FSA    -- all the functions exported by our FSA.hs file
import RE
  ( -- the type and constructors for RE
    RE(..)
    -- functions to build up REs
  , zero, one, char, (<|>), (<.>), star
    -- functions to generate strings from a RE and/or see if
    -- a String matches a RE
  , mset, match
  )
import Prelude
  ( -- that basic types that we've seen for integers, booleans, characters, and strings
    Int, Bool(True, False), Char, String
    -- the type constructor Maybe (and its data constructors, Just and Nothing)
  , Maybe(..)
    -- some basic arithmetic operations and relations:
  , (+), (-), (*), (<), (>), (==), maximum, minimum
    -- some basic boolean operations:
  , (&&), (||), not
    -- some list inspection and processing functions
  , take, drop, or, and, any, all, map, filter, concat, elem, (++)
    -- and some classes for showing and comparing things (don't worry about these)
  , Show, Eq, undefined
  )
import Data.Char (ord)
  
-- ========================================================================
-- make your changes below here:
-- ========================================================================

reverse :: [a] -> [a]
reverse []     = []
reverse (x:xs) = reverse xs ++ [x]

preambleDef :: FSA -> String -> [State]
preambleDef fsa u = [ q | q <- states fsa, any accepting (walk fsa u q) ]
  where accepting = \q -> elem q (finals fsa)

backStep :: FSA -> Char -> State -> [State]
backStep fsa c = \q -> [ s | (s,x,r) <- ttable fsa , r == q , x == c ]

backPedal :: FSA -> String -> State -> [State]
backPedal fsa     "" = return
backPedal fsa (c:cs) = backStep fsa c >=> backPedal fsa cs

preamble :: FSA -> String -> [State]
preamble fsa u = concat [ backPedal fsa (reverse u) q | q <- finals fsa ]

acceptsAgain :: FSA -> String -> Bool
acceptsAgain fsa u = any (\q -> elem q (starts fsa)) (preamble fsa u)

type SLGTransition = (Char, Char)
data SLG = SLGWith [Char] [Char] [Char] [SLGTransition]


slg1, slg2 :: SLG
slg1 = SLGWith "CV" "C" "V" [('C','C'), ('C','V'), ('V','V')]
slg2 = SLGWith "abc" "abc" "abc"
            [('a','a'),('b','b'),('c','c'),('a','b'),('b','a'),('a','c'),('c','a')]

acceptsSLG :: SLG -> String -> Bool
acceptsSLG (SLGWith _ is fs del) ""       = False
acceptsSLG (SLGWith _ is fs del) (c:cs) = elem c is && go c cs
  where
    go prev []       = elem prev fs
    go prev (x:xs)   = elem (prev,x) del && go x xs

slgToFSA :: SLG -> FSA
slgToFSA (SLGWith sigma i f delta) = FSAWith qs sigma [q0] finals trans
  where
    q0     = 0
    qOf c  = ord c
    qs     = q0 : map qOf sigma
    finals = map qOf f
    trans0 = [ (q0, c, qOf c) | c <- i ]
    transD = [ (qOf a, b, qOf b) | (a,b) <- delta ]
    trans  = trans0 ++ transD


shift :: Int -> EpsFSA -> EpsFSA
shift n (EpsWith qs0 sy0 i0 f0 d0) =
  EpsWith (map (+n) qs0) sy0 (map (+n) i0) (map (+n) f0)
          [ (s+n, x, t+n) | (s,x,t) <- d0 ]

unionFSA :: EpsFSA -> EpsFSA -> EpsFSA
unionFSA a@(EpsWith qs1 sy1 i1 f1 d1) b =
  EpsWith (q0:qs2) sy [q0] (f1 ++ f2) (d1 ++ d2 ++ epsStarts)
  where
    q0        = 0
    offset    = maximum (q0:qs1) + 1

    
    bShifted@(EpsWith qsB syB iB fB dB) = shift offset b

    
    qs2  = qsB
    f2   = fB
    d2   = dB
    i2   = iB

    epsStarts = [ (q0, Nothing, s) | s <- i1 ++ i2 ]
    sy        = sy1 ++ syB

catFSA :: EpsFSA -> EpsFSA -> EpsFSA
catFSA a@(EpsWith qs1 sy1 i1 f1 d1) b =
  EpsWith (qs1 ++ qsB) sy i1 fB (d1 ++ dB ++ bridges)
  where
    offset         = maximum qs1 + 1
    bShifted@(EpsWith qsB syB iB fB dB) = shift offset b

    bridges = [ (q, Nothing, s) | q <- f1, s <- iB ]
    sy      = sy1 ++ syB

repFSA :: EpsFSA -> EpsFSA
repFSA e@(EpsWith qs sy i f d) =
  let q0        = maximum qs + 1
      fromStart = [ (q0, Nothing, s) | s <- i ]
      toStart   = [ (q,  Nothing, s) | q <- f, s <- i ]
  in  EpsWith (q0:qs) sy [q0] (q0:f) (d ++ fromStart ++ toStart)


reToFSA :: RE -> EpsFSA
reToFSA Nil        = EpsWith [0] [] [0] [] []
reToFSA One        = EpsWith [0] [] [0] [0] []
reToFSA (Lit c)    = EpsWith [0,1] [c] [0] [1] [(0, Just c, 1)]
reToFSA (Alt r s)  = unionFSA (reToFSA r) (reToFSA s)
reToFSA (Cat r s)  = catFSA   (reToFSA r) (reToFSA s)
reToFSA (Rep r)    = repFSA   (reToFSA r)



re1 :: RE
re1 = (char 'a' <|> char 'b') <.> char 'c'

re2 :: RE
re2 = star re1

re3 :: RE
re3 = star (zero <.> char 'c')

re4 :: RE
re4 = (char 'a' <|> char 'b') <.> star (char 'c')
