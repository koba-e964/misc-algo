{-# LANGUAGE BangPatterns #-}
module Main where

import Control.Monad
import Control.Monad.ST
import Data.Array
import qualified Data.Array.IO as AIO
import Data.Bits
import Data.Array.ST
import Data.List
import Data.STRef
import Data.Set (Set)
import qualified Data.Set as Set
import Debug.Trace

data Logic = LFalse | LTrue | LDontCare deriving (Eq, Show)

data Cons =
  Cons Int -- number of variables
    (Array Int Logic) -- constraints

data Term = 
  Term Int -- bits
       Int -- mask

contain :: Term -> Int -> Bool
contain (Term bits mask) v = bits == mask .&. v

mergeable :: Term -> Term -> Bool
mergeable (Term b1 m1) (Term b2 m2) = m1 == m2 && popCount (b1 `xor` b2) == 1

merge :: Term -> Term -> Term
merge (Term b1 m1) (Term b2 _) = Term (b1 .&. b2) (m1 `xor` b1 `xor` b2)


instance Show Term where
  show (Term bits mask) = sub 0 where
    sub 20 = ""
    sub !n
      | testBit mask n = let rest = sub (n + 1) in (if testBit bits n then "" else "not ") ++ show n ++ (if null rest then "" else " & " ++ rest)
      | otherwise      = sub (n + 1)
solve :: Cons -> [Term]
solve (Cons n ary) =
  let
    mask = shiftL (1 :: Int) n - 1
    onPat = map fst $ filter ((== LTrue) . snd) $ assocs ary
    opSize = length onPat
    rem   = map (\x -> Term x mask) $ map fst $ filter ((/= LFalse) . snd) $ assocs ary
    sub terms = if null terms then [] else let (res, mt) = next terms in (mt ++ sub res)
    primImp = sub rem
    piSize = length primImp
    tbl = array ((0,0), (opSize - 1, piSize - 1))
       [((i,j), contain (primImp !! j) (onPat !! i)) | i <- [0 .. opSize - 1], j <- [0 .. piSize - 1]]
    necessary = petrick tbl
  in
    map (primImp !!) $ filter (\i -> testBit necessary i) $ [0.. piSize - 1]

petrick :: Array (Int, Int) Bool -> Int
petrick tbl = minimumBy (\x y -> popCount x `compare` popCount y) $ Set.toList $ runST $ do
  set <- newSTRef (Set.singleton 0)
  let (m, n) = snd $ bounds tbl
  forM_ [0..m] $ \i -> do
    newset <- newSTRef Set.empty
    forM_ [0..n] $ \j -> do
      setCont <- readSTRef set
      when (tbl ! (i, j)) $ modifySTRef newset (\s -> Set.foldl' (\acc e -> Set.insert (e .|. shiftL 1 j) acc) s setCont)
    readSTRef newset >>= writeSTRef set
  readSTRef set

next :: [Term] -> ([Term], [Term])
next terms = runST $ do
  let tSize = length terms
  paired <- newArray (0, tSize - 1) False :: ST s (STArray s Int Bool)
  let ary = listArray (0, tSize - 1) terms
  nextBuf <- newSTRef []
  forM_ [0 .. tSize - 1] $ \i -> do
    forM_ [i .. tSize - 1] $ \j -> do
      when (mergeable (ary ! i) (ary ! j)) $ do
        writeArray paired i True
        writeArray paired j True
        modifySTRef nextBuf ((merge (ary ! i) (ary ! j)) :)
  dec <- newSTRef []
  forM_ [0 .. tSize - 1] $ \i -> do
    isUsed <- readArray paired i
    when (not isUsed) $ modifySTRef dec ((ary ! i) :)
  do
    x <- readSTRef nextBuf
    y <- readSTRef dec
    return (x, y)


main = do
  n <- fmap read $ getLine
  ary <- AIO.newArray (0 :: Int, shiftL 1 n - 1) LDontCare :: IO (AIO.IOArray Int Logic)
  cons <- fmap (map (map read . words) . lines) getContents :: IO [[Int]]
  forM_ cons $ \dat -> do
    let result = dat !! (length dat - 1)
    let sub [] = 0
        sub (x : xs) = x + 2 * sub xs
    writeArray ary (sub (take (length dat - 1) dat)) (if result == 1 then LTrue else LFalse)
  aryCont <- freeze ary :: IO (Array Int Logic)
  print $ solve $ Cons n aryCont
