#run an iteration of the iterator and return whether it is finished
def done(iterator):
  try:
    iterator.next()
  except StopIteration:
    return True
  return False
