
   graph_file = '../graph.txt';
   train_file = '../training.txt';
   test_file = '../test_data/test_data_q1.txt';
   out_file = 'out.txt';
   N = 30;

   fid = fopen(out_file, 'w');
   fprintf(fid, 'testing\n');

   fprintf(2, 'reading Graph\n');
   graph = readGraph(graph_file);
   fprintf(2, 'reading Training\n');
   train = readTrain(train_file);
   fprintf(2, 'reading Testing\n');
   test = readTest(test_file);


   answer = zeros(size(test,2), 100);

   
   % fill matrix for svd
   matrix = ones(size(train, 2)+1, size(graph, 2))*-.5;
   for j = 1:size(train, 2) 
     matrix(j, train{j}.node) = train{j}.degree-0.5; 
   end
   j = 1;
      matrix(end, :) = 0; 
      matrix(end, test{j}) = 0.5;

      fprintf(2, 'start svd...\n');
      [U, S, V] = svd(matrix);
      S2 = S;
      S2(N+1:end, N+1:end) = 0;
      M = U*S2*V';

      
      tmp = zeros(size(graph,2), 2);
      tmp(:, 1) = M(end, :)';
      tmp(:, 2) = 1:size(graph,2);

      tmp = sortrows(tmp, -1);

      answer(j, :) = tmp(1:100, 2)';

      fprintf(fid, '%d ', answer(j, :));
      fprintf(fid, '\n');


   
   
