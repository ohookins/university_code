woman(ann).

woman(X) :- mother(X).

likes(Y,X) :- mother(Y),
		parent(Y,X),
		good(X).

good(jim).
mother(june).
mother(sue).
good(thomas).
parent(sue,thomas).
parent(june,pete).
