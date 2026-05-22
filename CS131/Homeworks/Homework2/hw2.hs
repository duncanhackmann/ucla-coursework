is_sum_divisible :: Int -> Int -> Int -> Bool
is_sum_divisible a b c =
    (total a b) `mod` c == 0

total :: Int -> Int -> Int
total a b = sum [a..b]

fib :: Int -> Int
fib n = _fib n 0 1
    where
        _fib :: Int -> Int -> Int -> Int
        _fib n a b = 
            if n == 0
                then a
            else _fib (n-1) b (a+b)


find_min :: [Int] -> Int
find_min (x:[]) = x
find_min (x:xs) = min x (find_min xs)

all_factors n = [x | x <- [1..n], n `mod` x == 0]

perfect_numbers = [n | n <- [0..], sum [x | x <- [1..n-1], n `mod` x == 0] == n]

is_palin :: String -> Bool
is_palin [] = True
is_palin (x:[]) = True
is_palin (x:xs) =
    if x == last xs
        then is_palin (init xs)
        else False

n_palin n = [x | x <- [0..n], (is_palin (show x)) == True]

drop_three :: [t] -> [t]
drop_three (_:_:_:xs) = (xs)
drop_three _ = []

count_occurrences :: Eq a => [a] -> [a] -> Int
count_occurrences [] _ = 1
count_occurrences _ [] = 0
count_occurrences (x:xs) (y:ys)
    | x == y    = count_occurrences xs ys + count_occurrences (x:xs) ys
    | otherwise = count_occurrences (x:xs) ys

fiball :: Int -> [Int]
fiball n = reverse (helper n 0 1 [0])
  where
    helper 0 _ _ acc = acc
    helper k a b acc = helper (k-1) b (a+b) (b:acc)

rle :: [Int] -> [(Int, Int)]
rle [] = []
rle (x:xs) = helper x 1 xs
  where
    helper curr count [] = [(curr, count)]
    helper curr count (y:ys)
      | y == curr = helper curr (count + 1) ys
      | otherwise = (curr, count) : helper y 1 ys


only_odds :: [[Integer]] -> [[Integer]]
only_odds arrs = [xs | xs <- arrs, all (\x -> x `mod` 2 /= 0) xs]
