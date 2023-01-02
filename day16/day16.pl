%
% Advent of Code 2022 - Day 16
%


% Valve properties: v(Name, Flow)
% Connections: s(Valve1, Valve2)

identifier_char(Char) --> [Char],
	{	char_type(Char, alpha)
	}.

identifier_chars([HChar | TChars]) --> identifier_char(HChar), identifier_chars(TChars).
identifier_chars([Char]) --> identifier_char(Char).

vname(Name) --> identifier_chars(IDChars),
	{	string_chars(Name, IDChars)
	}.

flow(F) --> digits(Digits),
	{	string_chars(Str, Digits),
		number_string(F, Str)
	}


valve_list(VL) --> 

optional_s --> 

valve_spec --> 'Valve', vname(VN), 'has flow rate=', flow, '; tunnels lead to valve', optional_s, valve_list.

