{-# LANGUAGE NoImplicitPrelude #-}

-- ========================================================================
-- do not change anything in this module header
-- ========================================================================

module HW3 where 


-- The only bits of Haskell that will be available to you in this assignment
-- are the things you see imported here at the top. In particular, the only
-- pre-defined Haskell functions and types that you'll be able to use are the
-- ones you see in `import Prelude` block below:

import FSA -- the functions exported by our FSA.hs file
import Prelude
  ( -- that basic types that we've seen for integers, booleans, characters, and strings
    Int, Bool(True, False), Char, String
    -- some basic arithmetic operations and relations:
  , (+), (-), (*), (<), (>), (==)
    -- some basic boolean operations:
  , (&&), (||), not
    -- some list maniuplating functions
  , take, drop, or, and, any, all, map, concat, elem, (++)
    -- and some classes for showing and comparing things (don't worry about these)
  , Show, Eq, undefined
  )
  
-- ========================================================================
-- make your changes below here:
-- ========================================================================


myFSA :: FSA 
myFSA = FSAWith states syms i f delta 
  where
    states = [51, 22, 63, 34]  -- q0, q1, q2, q3
    syms   = ['a', 'b']
    i      = [51]
    f      = [34]
    delta  = [ (51, 'b', 22), (51, 'a', 51)
             , (22, 'a', 63), (22, 'b', 22)
             , (63, 'b', 34), (63, 'a', 51)
             , (34, 'a', 34), (34, 'b', 34)
             ]

testSuite :: [String]
testSuite = [str1, str2, str3, str4, str5]
  where str1 = "bab"
        str2 = "aa"
        str3 = "babba"
        str4 = "bbbabb"
        str5 = "bbbabbb"

testResults :: [(String, Bool)]
testResults = [ (s, accepts myFSA s) | s <- testSuite ]

{-

myFSA accepts exactly the strings over the alphabet {a,b}
that contain the substring "bab" at least once, no matter
what appears before or after that substring.

-}

-- myRE :: RE
-- myRE = undefined

evenas :: FSA
evenas = FSAWith states syms i f delta
  where states = [0,1]
        syms   = ['a','b']
        i      = [0]
        f      = [0]
        delta  = [ (0,'a',1), (0,'b',0)
                , (1,'a',0), (1,'b',1) ]


oddas :: FSA
oddas = FSAWith states syms i f delta
  where
    states = [0,1]
    syms   = ['a','b']
    i      = [0]
    f      = [1]
    delta  = [ (0,'a',1), (0,'b',0)
             , (1,'a',0), (1,'b',1) ]

testEven :: Bool
testEven = all (accepts evenas) suite
  where suite = ["aa","aba","abbabbbb","","aaaaaabaa"]

testOdd :: Bool
testOdd = all (accepts oddas) suite
  where suite = ["aaa","ba","abbabbba","a","aaaaaaba"]

fsaHarmony :: FSA
fsaHarmony = FSAWith states syms i f delta
  where
    states = [0,1,2,3]
    syms   = ['k','i','u',' ']
    i      = [0]
    f      = [0,1,2]                        
    delta  = [ (0,'k',0), (0,'i',1), (0,'u',2), (0,' ',0)
             , (1,'k',1), (1,'i',1), (1,'u',3), (1,' ',0)
             , (2,'k',2), (2,'u',2), (2,'i',3), (2,' ',0)
             , (3,'k',3), (3,'i',3), (3,'u',3), (3,' ',3) ]

fsaKI :: FSA
fsaKI = FSAWith states syms i f delta
  where
    states = [0,1,2]
    syms   = ['k','i','u',' ']
    i      = [0]
    f      = [0,1]
    delta  = [ (0,'k',1), (0,'i',2), (0,'u',0), (0,' ',0)
             , (1,'k',1), (1,'i',0), (1,'u',0), (1,' ',0)
             , (2,'k',2), (2,'i',2), (2,'u',2), (2,' ',2) ]

requireCs :: Int -> FSA
requireCs n = FSAWith states syms i f (deltaC ++ deltaV)
  where
    states = [0 .. n + 1]
    syms   = ['c','v']
    i      = [0]
    f      = [n]
    deltaC = [ (k, 'c', k + 1) | k <- [0 .. n - 1] ] ++
             [ (n, 'c', n + 1) ] ++
             [ (n + 1, 'c', n + 1) ]
    deltaV = [ (k, 'v', k) | k <- states ]

