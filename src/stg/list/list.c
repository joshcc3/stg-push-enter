#include "list.h"
#include <assert.h>

ref map_thunk1(ref bindings)
{
  get bindings to f
  get bindings to x

  if(f is a thunk)
  {
    push update frame
    pop update(enter thunk)
    map_thunk1(bindings);
  }
  else if(f is a pap)
  {
    push x
    unroll pap on to stack
    return enter fs code
  }
  else if(if is a func)
  {
    push x
    return enter fs code
  }
  else assert(false);
}

ref map_thunk2(ref bindings)
{
  get bindings to f
  get bindings to xs
  return map_fast(f, xs);
}

/*
map f l = case l of
            Cons x xs -> let x' = THUNK (f x) in
                         let f' = THUNK (map f xs) in
                         let c' = Cons x' f'
                         in c'
*/
ref map_fast(ref function, ref list)
{
    declare thunk1
    bind x' to it
    declare thunk2
    bind f' to it
    declare constructor
    return constructor
}

ref map_slow(ref null)
{
  assert (su - stack_pointer < sizeof(void*)*2)
  if(su - stack_pointer == sizeof(void*)*2)
  {
    arg1 = pop
    arg2 = pop
    return map_fast(arg1, arg2)
  }
  else
  {
     // at least 1 arg must be present
     arg1 = pop
     build a pap with arg1
     return pap
  }
}

ref head_case_cont(ref bindings)
{
    arg = get l from bindings
    if(constructor)
    {
      must be cons
      return ref to x in l
    }
    else
    {
      must be a thunk
      push the case frame refed to case_cont
      push the update frame to update l
      pop case(pop update(enter thunks code))
    }
}
/*
    head :: [a] -> a
    head l = case l of
                Cons x xs -> x
*/
ref head_fast(ref list)
{
    bind l to list in bindings
    head_case_cont(bindings)
}

ref head_slow(ref null)
{
    // no need to perform an argument satisfaction check because the only way this is called is when its applied to its argument
    pop arg from stack
    head_fast(arg)
}


ref tail_case_cont(ref bindings)
{
   if(l is a constructor)
   {
      assert its a cons
      return (get xs for l)
   }
   else
   {
     must be a thunk
     push case frame (tail case cont)
     push update frame(l)
     pop case(pop update(enter thunk code))
   }
}

/*
  tail :: [a] -> [a]
  tail l = case l of
              Cons x xs -> xs
*/
ref tail_fast(ref list)
{
  bind l to list in bindings
  tail_case_cont(bindings);
}

ref tail_slow(ref list)
{
  pop arg from stack
  tail_fast(arg);
}