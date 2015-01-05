function train = readTrain(train_file)
   fid = fopen(train_file);
   idea = -1;
   j = 0;
   while ~feof(fid)
      line = fgets(fid);
      values = sscanf(line, '%d %d %d/%d/%d %f');
      tmp.node = values(1);
      tmp.degree = values(end);

      if idea ~= values(2)
         idea = values(2);
         j = j+1;
      end

      train{j} = tmp;
   end
end

