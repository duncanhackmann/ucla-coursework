{-# LANGUAGE NoImplicitPrelude #-}

-- ========================================================================
-- do not change anything in this preamble module header
-- ========================================================================

module HW2 where 


-- The only bits of Haskell that will be available to you in this assignment
-- are the things you see imported here at the top. In particular, the only
-- pre-defined Haskell functions and types that you'll be able to use are the
-- ones you see in `import Prelude` block below

import Prelude
  ( -- the basic types that we've seen for integers, booleans, characters, and strings
    Int, Bool(True, False), Char, String
    -- some basic arithmetic operations and relations:
  , (+), (-), (*), (<), (>), (==)
    -- some basic boolean operations:
  , (&&), (||), not
    -- some basic list processing functions, which you might want while checking your 
  , take, drop, (++)
    -- and some classes for showing and comparing things (don't worry about these)
  , Show, Eq, undefined
  )

import RE
  ( -- the RE type
    RE
    -- ways to build small REs as building blocks
  , zero, one, char, str
    -- functions to build larger REs out of smaller ones
  , (<|>), (<.>), star
    -- functions to generate and/or match strings from an RE
  , mset, match
  )

-- ========================================================================
-- make your changes below here:
-- ========================================================================

data Nat = Z | S Nat
  deriving Show

toInt :: Nat -> Int
toInt Z     = 0
toInt (S n) = 1 + toInt n

toNat :: Int -> Nat
toNat 0 = Z
toNat n = S ( toNat(n-1))

add :: Nat -> Nat -> Nat
add Z     n = n
add (S m) n = add m (S n)

mul :: Nat -> Nat -> Nat
mul Z     n = Z
mul (S m) n = add n (mul m n)

equal :: Nat -> Nat -> Bool
equal Z Z = True
equal Z (S n) = False
equal (S n) Z = False
equal (S n) (S m) = equal n m

count :: (Int -> Bool) -> [Int] -> Nat
count p []     = Z
count p (x:xs) =
  if p x
     then S (count p xs)
     else     count p xs
  

append :: Char -> String -> String
append c ""     = [c]
append c (u:us) = u : append c us

reverse :: String -> String
reverse ""     = ""
reverse (u:us) = append u (reverse us)

anyOf :: [Char] -> RE
anyOf (c:cs) = char c <|> anyOf cs

anych, alpha, lower, upper, digit :: RE
anych = anyOf (['!'..'~'] ++ " \n\r\t") -- matches any single character
lower = anyOf ['a'..'z'] -- matches any lowercase letter
upper = anyOf ['A'..'Z'] -- matches any uppercase letter
alpha = lower <|> upper -- matches any letter
digit = anyOf ['0'..'9'] -- matches any digit

startsWithS :: RE
startsWithS = char 'S' <.> star (alpha <|> digit)

evenLetters :: RE
evenLetters = star ( digit <|> (alpha <.> star digit <.> alpha) )

capitalized :: RE
capitalized = upper <.> star lower

termZ :: RE
termZ = (char 'z' <.> star alpha) <|> (star alpha <.> char 'z')

oneI :: RE
oneI = star anych <.> (char 'i' <|> char 'I') <.> star anych
