
-- ========================================================================
-- do not change anything in this module header
-- ========================================================================

module HW5 where 


-- The only bits of Haskell that will be available to you in this assignment
-- are the things you see imported here at the top. In particular, the only
-- pre-defined Haskell functions and types that you'll be able to use are the
-- ones you see in `import Prelude` block.

import FST     -- all the functions exported by our FST.hs file
import Algebra -- all types and classes in the Algebra.hs file
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
  , take, drop, or, and, any, all, map, filter, elem, (++), (.)
    -- and some classes for showing and comparing things (don't worry about these)
  , Show, Eq, undefined
  )
  
-- ========================================================================
-- make your changes below here:
-- ========================================================================

identityFST :: [Char] -> FST Char String
identityFST sigma =
  let q0 = 0 :: State
  in FSTWith [(q0, "")] [(q0, "")]
       [ (q0,(c,[c]),q0) | c <- sigma ]

asciiChars :: [Char]
asciiChars = ['\x20' .. '\x7e']   -- ' ' to '~'

fstNAP, fstNAM, fstNAC, fstEMM :: FST Char String
fstNAP = identityFST asciiChars
fstNAM = fstNAP
fstNAC = fstNAP
fstEMM = fstNAP

nacemm :: String -> [String]
nacemm u = return u

inmitTest :: [String]
inmitTest = nacemm "inmit"

fsaTR :: FSA -> FST Char [State]
fsaTR (FSAWith _qs _sigma initials finals delta) =
  FSTWith initials' finals' arcs
  where
    initials' = [(q,[q]) | q <- initials]
    finals'   = [(q,[])  | q <- finals]
    arcs      = [ (s, (c,[t]), t) | (s,c,t) <- delta ]

dp :: Semiring w => [w] -> [w] -> w
dp [] _          = summarize []
dp _  []         = summarize []
dp (x:xs) (y:ys) = (x <*> y) <+> dp xs ys

exn :: Monoid w => w -> Int -> w
exn w n
  | n <= 0    = unit
  | True = w <*> exn w (n-1)

costA, costB :: Cost
costA = Fin 17
costB = Inf

ac :: Cost -> Cost -> Cost
ac (Fin x) (Fin y) = Fin (if x < y then x else y)
ac Inf     c       = c
ac c       Inf     = c

mc :: Cost -> Cost -> Cost
mc (Fin x) (Fin y) = Fin (x + y)
mc _       _       = Inf

instance Monoid Cost where
  (<*>) = mc
  unit  = Fin 0
  
instance Semiring Cost where
  summarize []     = Inf
  summarize (c:cs) = minCost c cs
    where
      minCost acc []     = acc
      minCost acc (d:ds) = minCost (ac acc d) ds
  

fsaCT :: FSA -> FST Char Count
fsaCT (FSAWith _qs _sigma initials finals delta) =
  FSTWith initials' finals' arcs
  where
    initials' = [(q,1) | q <- initials]
    finals'   = [(q,1) | q <- finals]
    arcs      = [ (s,(c,1),t) | (s,c,t) <- delta ]
