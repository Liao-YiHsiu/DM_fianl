

import sys

if __name__ == '__main__':
    if len(sys.argv) != 3:
	sys.stderr.write("Usage: python eval.py <prediction.txt> <answer.txt>\n" )
        sys.exit(0)

    predict = open( sys.argv[1] , 'r' )
    answers = open( sys.argv[2] , 'r' )

    correct = 0
    predict_count = 0
    answer_count = 0
    while True:
        pre_line = predict.readline()
	ans_line = answers.readline()
	exit = False
	if not pre_line:
	    sys.stderr.write("EOF for " + sys.argv[1] + "\n" );
	    exit = True
	if not ans_line:
	    sys.stderr.write("EOF for " + sys.argv[2] + "\n" );
	    exit = True
	if exit:
	    break
    
    	predictions = pre_line.rstrip("\n").split() 
	adopters    = ans_line.rstrip("\n").split() 
	#print predictions
	for i in range(len(predictions) ):
	    if predictions[i] in adopters:
		correct += 1
	predict_count += len(predictions)
	answer_count += len(adopters)

    if predict_count == 0 or answer_count == 0:
	sys.stderr.write( "Error: no predict or answer found.\n")
	sys.exit(0)

    precision = float(correct)/float(predict_count) 
    print( "precision = %d / %d , %4f" %( correct, predict_count, precision ) )
    recall = float(correct)/float(answer_count) 
    print( "recall    = %d / %d , %4f" %( correct, answer_count, recall ) )
    f_measure = 2.0 / ((1.0/precision) + (1.0/recall) )
    print( "f-measure = %4f" %( f_measure ) )
