parent(tom,bob).
parent(pam,bob).
parent(tom,liz).
parent(bob,ann).
parent(bob,pat).
parent(pat,jim).

female(pam).
female(liz).
female(ann).
female(pat).

offspring(Y,X) :- parent(X,Y).

predecessor(X,Z) :-
	parent(X,Z).

predecessor(X,Z) :-
	parent(X,Y),
	predecessor(Y,Z).

sister(X,Y) :-
	parent(Z,X),
	parent(Z,Y),
	female(X),
	different(X,Y).

different(X,Y) :- X \= Y.

