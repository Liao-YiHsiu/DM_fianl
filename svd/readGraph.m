function graph = readGraph(graph_file)
   fid = fopen(graph_file);
   while ~feof(fid)
      line = fgets(fid);
      vector = sscanf(line, '%d');
      graph{vector(1)} = vector(2:end)';
   end
end
