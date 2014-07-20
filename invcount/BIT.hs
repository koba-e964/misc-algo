module BIT where

import Data.Bits
import Data.Int
import Data.Array
import Data.Array.MArray

data BIT a e = BIT Int32 (a Int32 e)

nearestPow2 :: Int32 -> Int32
nearestPow2 s
  | s < 0 = undefined
  | s == 0 = 1
  | otherwise = sub s
    where
     sub k
      | k == k .&. (-k) = k
      | otherwise = sub ((k .&. (-k)) + k)

newBIT :: (MArray a e m, Num e) => Int32 -> m (BIT a e)

newBIT size = do
  let n = nearestPow2 size
  ary <- newArray (1,n) 0 
  return $ BIT n ary

freezeBIT :: (MArray a e m) => BIT a e -> m (Array Int32 e)
freezeBIT (BIT _ ary) = freeze ary

accumBIT :: (MArray a e m, Num e) => BIT a e -> Int32 -> m e
accumBIT (BIT _ ary) i = do
  let sub 0 = return 0
      sub i' = do 
        val <- readArray ary i'
        acc <- sub (i' .&. (i' - 1))
        return $ val + acc
  sub i

addBIT :: (MArray a e m, Num e) => BIT a e -> Int32 -> e -> m ()
addBIT (BIT n ary) ix val = do
  let sub i'
       | i' > n = return ()
       | otherwise = do 
         oldval <- readArray ary i'
         writeArray ary i' (oldval + val)
         sub (i' + (i' .&. (-i')))
  sub ix


