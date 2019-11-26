procedure AAA;
var L: integer;
begin
   L:=5;
end;

var T,M: integer;
begin
   T:=5;
   M:=T-3;
   if T>1 then M:=T-3
   else M:=1;
   AAA;
end.
