%
% Advent of Code 2022 - Day 13.
% 

:- working_directory(_, 'C:/Users/stephen.mccoy/github/aoc2022/day13/').

:- dynamic packets/3.


% List concatenation.
conc([], L, L).
conc([H | Tail], L1, [H | L2]) :-
	conc(Tail, L1, L2).

index_of(Item, [Item | _], 1) :- !.
index_of(Item, [_ | Tail], N) :-
	index_of(Item, Tail, M),
	N is M + 1.


grab_next_non_blank_line(Stream, Line) :-
	read_line_to_string(Stream, L1),
	L1 \== end_of_file,
	(	L1 == "", grab_next_non_blank_line(Stream, Line) ;
		Line = L1
	).

grab_next_term(Stream, Term) :-
	grab_next_non_blank_line(Stream, Line),
	read_term_from_atom(Line, Term, []).

read_input_line(Stream, RowNum) :-
	grab_next_term(Stream, Term1),
	grab_next_term(Stream, Term2),
	assertz(packets(RowNum, Term1, Term2)).

read_input_lines(Stream, RowNum) :-
	read_input_line(Stream, RowNum),
	!,
	N is RowNum + 1,
	read_input_lines(Stream, N).
read_input_lines(_,_).

read_input_data(FileName) :-
	retractall(packets(_, _, _)),
	open(FileName, read, Stream),
	read_input_lines(Stream, 1),
	close(Stream).


compare_terms(T1, T2, Result) :-
	integer(T1),
	integer(T2), !,
	compare_integers(T1, T2, Result).

compare_terms(I1, Term2, Result) :-
	integer(I1), !,
	compare_terms([I1], Term2, Result).

compare_terms(Term1, I2, Result) :-
	integer(I2), !,
	compare_terms(Term1, [I2], Result).

compare_terms([], [_ | _], yes)	:- !.
compare_terms([_ | _], [], no) 	:- !.
compare_terms([], [], maybe)	:- !.
compare_terms([H1 | Tail1], [H2 | Tail2], Result) :-
	compare_terms(H1, H2, Res1),
	(	Res1 == maybe,
		compare_terms(Tail1, Tail2, Result)
	;
		Result = Res1
	),	!.

compare_terms(_, _, no).

% Rules for comparing integers above.
compare_integers(I1, I2, yes) :-	I1 < I2, !.
compare_integers(I1, I2, no) :-		I1 > I2, !.
compare_integers(I1, I1, maybe).

% Predicate used for sorting in part 2.
sort_predicate(<, T1, T2) :-	compare_terms(T1, T2, yes).
sort_predicate(>, T1, T2) :-	compare_terms(T1, T2, no).
sort_predicate(=, T1, T2) :-	compare_terms(T1, T2, maybe).


solve_for(FileName) :-
	read_input_data(FileName),
	findall(I, (
		packets(I, T1, T2), 
		compare_terms(T1, T2, yes)
	),
	IndexList),
	format('Indices of correct order pairs: ~w\n', [IndexList]),
	sum_list(IndexList, Sum),
	format('Index sum = ~w\n', [Sum]).

spit_list([]).
spit_list([Head | Tail]) :-
	writeln(Head),
	spit_list(Tail).

part2_for(FileName) :- 
	read_input_data(FileName),
	% Get all terms from input file.
	findall(T1, (packets(_, T1, _)), List1),
	findall(T2, (packets(_, _, T2)), List2),
	conc([[[2]], [[6]] | List1], List2, UnsortedTerms),
	length(UnsortedTerms, N),
	format('Found ~w unsorted terms in input.\n', [N]),
	predsort(sort_predicate, UnsortedTerms, SortedTerms),
	writeln('Sorted.'),
	spit_list(SortedTerms),
	index_of([[2]], SortedTerms, Index1),
	index_of([[6]], SortedTerms, Index2),
	format('Divider packets found at indices ~w and ~w.\n', [Index1, Index2]),
	Product is Index1 * Index2,
	format('Decoder key = ~w\n', [Product]).

day13_part1 :-
	writeln('Advent of Code 2022.\nDay 13, Part 1.'),
	writeln('Test run...'),
	solve_for('test13.txt'),

	writeln('Full run...'),
	solve_for('day13.txt').

day13_part2 :- 
	writeln('Advent of Code 2022.\nDay 13, Part 2.'),
	writeln('Test run...'),
	part2_for('test13.txt'),

	writeln('Full run...'),
	part2_for('day13.txt').

