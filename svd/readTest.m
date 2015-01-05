function test = readTest(test_file)
   fid = fopen(test_file);
   j = 1;
   
   while ~feof(fid)
      line = fgets(fid);
      Adpts = sscanf(line, '%d');

      test{j} = Adpts;
      j = j+1;
   end
end
