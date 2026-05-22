
-- ========================================================================
-- do not change anything in this module header
-- ========================================================================

module HW8 where 


-- The only bits of the standard Haskell library that will be available to you
-- are the things you see imported here:
import FTA
import Prelude
  ( -- the basic types that we've seen for integers, booleans, characters, and strings
    Int, Bool(True, False), Char, String
    -- the type constructor Maybe (and its data constructors, Just and Nothing)
  , Maybe(..)
    -- some basic arithmetic operations and relations:
  , (+), (-), (*), (<), (>), (==), (>=), (<=), maximum, minimum, max, min, sum, product
    -- some basic boolean operations:
  , (&&), (||), not
    -- some list inspection and processing functions
  , take, drop, or, and, any, all, map, filter, concat, elem, (++), (.), words
    -- and some classes for showing and comparing things (don't worry about these)
  , Show, Eq, undefined
  )
  
-- ========================================================================
-- make your changes below here:
-- ========================================================================


-- the configuration's memory is just a list of chunks (nonterminals)
type Stack = [Cat]
-- the buffer is a list of words
type Buffer = [String]
-- the configuration is a pair of a current stack and a current buffer
type Config = (Stack, Buffer)

data Cat = S | DP | D | PS | NP | N | SRC | ORC | PRC | C | VP | VT | VI | PP | P
  deriving (Eq, Show)

gtbu :: [Config]
gtbu =
  [ ([], ["the","bird","watched","from","the","tree","sang"])  -- starting config
  , ([D],         ["bird","watched","from","the","tree","sang"])  -- shift "the"
  , ([D,N],       ["watched","from","the","tree","sang"])        -- shift "bird"
  , ([D,N,VT],    ["from","the","tree","sang"])                  -- shift "watched"
  , ([D,N,VT,P],  ["the","tree","sang"])                         -- shift "from"
  , ([D,N,VT,P,D],["tree","sang"])                               -- shift "the"
  , ([D,N,VT,P,D,N],["sang"])                                    -- shift "tree"
  , ([D,N,VT,P,NP],["sang"])                                     -- reduce N → NP
  , ([D,N,VT,PP], ["sang"])                                      -- reduce P NP → PP
  , ([D,N,PRC],  ["sang"])                                       -- reduce VT PP → PRC
  , ([D,NP],     ["sang"])                                       -- reduce N PRC → NP
  , ([DP],       ["sang"])                                       -- reduce D NP → DP
  , ([DP,VI],    [])                                             -- shift "sang"
  , ([DP,VP],    [])                                            -- reduce VI → VP
  , ([S],        [])                                            -- reduce DP VP → S
  ]

{-------------------------------
At step 7 (when the stack is `[D,N,VT,P,D,N]` and the buffer is `["sang"]`), one could have reduced `N` → `NP` and `D NP` → `DP` prematurely, treating “the bird” as a complete DP before forming the relative‐clause “watched from the tree.” That wrong turn would have meant parsing “watched” as the main verb (VI) rather than part of a PRC under NP.

-------------------------------}

gttd :: [Config]
gttd =
  [ ([S], ["the","bird","watched","from","the","tree","sang"])    -- starting config
  , ([DP,VP], ["the","bird","watched","from","the","tree","sang"])   -- predict S → DP VP
  , ([D,NP,VP], ["the","bird","watched","from","the","tree","sang"]) -- predict DP → D NP
  , ([NP,VP], ["bird","watched","from","the","tree","sang"])         -- match D
  , ([N,PRC,VP], ["bird","watched","from","the","tree","sang"])      -- predict NP → N PRC
  , ([PRC,VP], ["watched","from","the","tree","sang"])               -- match N
  , ([VT,PP,VP], ["watched","from","the","tree","sang"])             -- predict PRC → VT PP
  , ([PP,VP], ["from","the","tree","sang"])                           -- match VT
  , ([P,DP,VP], ["from","the","tree","sang"])                         -- predict PP → P DP
  , ([DP,VP], ["the","tree","sang"])                                   -- match P
  , ([D,NP,VP], ["the","tree","sang"])                                -- predict DP → D NP
  , ([NP,VP], ["tree","sang"])                                        -- match D
  , ([N,VP], ["tree","sang"])                                         -- predict NP → N
  , ([VP], ["sang"])                                                  -- match N
  , ([VI], ["sang"])                                                  -- predict VP → VI
  , ([], [])                                                          -- match VI
  ]

{-------------------------------
At the NP node after matching “the bird,” one could have predicted NP → N (omitting PRC), treating “bird” as a complete NP. That wrong turn would parse “watched” as the main verb (VI) rather than as part of a PRC.

-------------------------------}

{-------------------------------
If humans use a bottom-up parser, we should see eye‐movement regressions exactly when the parser completes the relative‐clause reduction (at “tree”) and then fails on “sang,” causing backtracking. If humans use a top-down parser, we should see regressions when a prediction (expecting a PRC after “bird”) fails at “sang.” In each case, the buffer at the crucial moment (either “sang” or “tree sang”) differs.

-------------------------------}

walkFTA :: Eq q => [Transition q] -> Tree String -> [q]
walkFTA delta (Node c cs) =
  let
      -- cart computes the Cartesian product of states over all child subtrees
      cart ts =
        case ts of
          []     -> [[]]
          (t:ts') ->
            [ s : ss
            | s  <- walkFTA delta t
            , ss <- cart ts'
            ]

      combos = cart cs

      matchingStates =
        [ q'
        | statesList <- combos
        , (lhs, sym, q') <- delta
        , sym == c
        , lhs == statesList
        ]
  in matchingStates

generates :: Eq q => FTA q -> Tree String -> Bool
generates m t =
  let finals     = getFinals m
      allStates  = walkFTA (getDelta m) t
  in any (`elem` finals) allStates


data WH1State = No1 | Q1 | W1 | OK1
  deriving (Eq, Show)

leafTrans1 :: [Transition WH1State]
leafTrans1 =
  [ ([], w, Q1)   | w <- qWords ]       -- “Q” leaf ⇒ Q1
  ++ [ ([], w, W1)  | w <- whWords ]    -- any wh leaf ⇒ W1
  ++ [ ([], w, No1) | w <- plainWords ] -- any other leaf ⇒ No1

combine1 :: WH1State -> WH1State -> WH1State
combine1 s1 s2 = case (s1, s2) of
  (OK1, _)        -> OK1
  (_, OK1)        -> OK1
  (Q1, W1)        -> OK1
  (W1, Q1)        -> OK1
  (Q1, No1)       -> Q1
  (No1, Q1)       -> Q1
  (Q1, Q1)        -> Q1
  (W1, No1)       -> W1
  (No1, W1)       -> W1
  (W1, W1)        -> W1
  (No1, No1)      -> No1
  _               -> No1  -- (covers any remaining case)

internalTrans1 :: [Transition WH1State]
internalTrans1 =
  [ ([s1, s2], ".", combine1 s1 s2)
  | s1 <- [No1, Q1, W1, OK1]
  , s2 <- [No1, Q1, W1, OK1]
  ]

finals1 :: [WH1State]
finals1 = [No1, OK1]

wh1FTA :: FTA WH1State
wh1FTA = FTAWith finals1 (leafTrans1 ++ internalTrans1)



data WH2State = No2 | Q2 | W2 | OK2 | BAD2
  deriving (Eq, Show)

leafTrans2 :: [Transition WH2State]
leafTrans2 =
  [ ([], w, Q2)   | w <- qWords ]
  ++ [ ([], w, W2)  | w <- whWords ]
  ++ [ ([], w, No2) | w <- plainWords ]

combine2_dot :: WH2State -> WH2State -> WH2State
combine2_dot s1 s2 = case (s1, s2) of
  (BAD2, _)       -> BAD2
  (_, BAD2)       -> BAD2
  (OK2, _)        -> OK2
  (_, OK2)        -> OK2
  (Q2, W2)        -> OK2
  (W2, Q2)        -> OK2
  (Q2, No2)       -> Q2
  (No2, Q2)       -> Q2
  (Q2, Q2)        -> Q2
  (W2, No2)       -> W2
  (No2, W2)       -> W2
  (W2, W2)        -> W2
  (No2, No2)      -> No2
  _               -> No2

internalDot2 :: [Transition WH2State]
internalDot2 =
  [ ([s1, s2], ".", combine2_dot s1 s2)
  | s1 <- [No2, Q2, W2, OK2, BAD2]
  , s2 <- [No2, Q2, W2, OK2, BAD2]
  ]

combine2_star :: WH2State -> WH2State -> WH2State
combine2_star s1 s2 = case (s1, s2) of
  (BAD2, _)     -> BAD2
  (_, BAD2)     -> BAD2
  (W2, _)       -> BAD2
  (_, W2)       -> BAD2
  _             -> No2

internalStar2 :: [Transition WH2State]
internalStar2 =
  [ ([s1, s2], "*", combine2_star s1 s2)
  | s1 <- [No2, Q2, W2, OK2, BAD2]
  , s2 <- [No2, Q2, W2, OK2, BAD2]
  ]


finals2 :: [WH2State]
finals2 = [No2, OK2]

wh2FTA :: FTA WH2State
wh2FTA = FTAWith finals2 (leafTrans2 ++ internalDot2 ++ internalStar2)
