largest :: String -> String -> String
largest str1 str2
    | str1 > str2 = str1
    | str1 < str2 = str2
    | str1 == str2 = str1

reflect :: Integer -> Integer
reflect 0 = 0
reflect num
  | num < 0 = (-1) + reflect (num+1)
  | num > 0 = 1 + reflect (num-1)



is_even :: Integer -> Bool
is_even 0 = True
is_even num = is_odd (num-1)

is_odd :: Integer -> Bool
is_odd 0 = False
is_odd num = is_even (num-1)

quad :: Double -> Double -> Double -> (Double, Double)
quad a b c =
  let disc = b^2 - 4 * a * c
  in if a == 0 || disc < 0
       then (0, 0)
       else
         let root1 = (-b + sqrt disc) / (2 * a)
             root2 = (-b - sqrt disc) / (2 * a)
         in (root1, root2)
