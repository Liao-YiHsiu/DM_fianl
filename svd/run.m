function answer = run(graph_file, train_file, test_file, out_file, N) 
   if nargin < 5
      N = [1 2 5 10 15 20 25 30 50 100 150 200];
   end
%   cluster = parcluster('local');
%   cluster.NumWorkers=8;
%   saveProfile(cluster);
%   matlabpool open 8;


   fprintf(2, 'reading Graph\n');
   graph = readGraph(graph_file);
   fprintf(2, 'reading Training\n');
   train = readTrain(train_file);
   fprintf(2, 'reading Testing\n');
   test = readTest(test_file);


   answer = zeros(size(test,2), 100);

   
   % fill matrix for svd
   matrix = 0.5*ones(size(train, 2)+size(test, 2), size(graph, 2));

   for j = 1:size(train, 2) 
     matrix(j, train{j}.node) = train{j}.degree; 
   end

   for j = 1:size(test, 2)
      matrix(size(train,2)+j, test{j}) = 0.8;
   end

   fprintf(2, 'start svd...\n');
   [U, S, V] = svd(matrix);

   for n = N
      fid = fopen(strcat(out_file, '_', int2str(n)), 'w');

      S2 = S;
      S2(n+1:end, n+1:end) = 0;
      M = U*S2*V';

      for j = 1:size(test, 2)
         M(size(train, 2)+j, test{j}) = -100;

         tmp = zeros(size(graph,2), 2);
         tmp(:, 1) = M(size(train,2)+j, :)';
         tmp(:, 2) = 1:size(graph,2);

         tmp = sortrows(tmp, -1);

         answer(j, :) = tmp(1:100, 2)';

         fprintf(fid, '%d ', answer(j, :));
         fprintf(fid, '\n');
      end
   end

%   matlabpool close

end
