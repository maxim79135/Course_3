procedure AAA;
var K,L:integer;
begin
    K:=4;
    L:=K+4;
end;
var A,B,C:integer;
begin
    AAA;
    A:=B+4;
    B:=B-4;
    if C<10 then C:=B-4
    else C:=0;
    A:=(C-15)+(B+4);
end.
