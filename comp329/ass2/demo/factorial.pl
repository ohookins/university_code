factorial(1,1).

factorial(N,F) :-
  write('Try fact of '), write(N), nl,
  N > 1, N1 is N - 1, factorial(N1,F1),
  F is N * F1,
  write('Fact of '), write(N),
  write(' is '), write(F), nl.
