function count=str2file(file, strOut)

%STR2FILE writes a string to a text file
%
% Syntax: y=file2str(file, string)
%
% Description
%   file - the file to write to
%   strOut - the string to write
%   count - number of bytes written to file
%
% Jeffrey Minton, jmint580@live.kutztown.edu, 16 November 2010, MAT4NAT Tbox


fid=fopen(file,'w');
count = fprintf(fid, strOut);
fclose(fid);