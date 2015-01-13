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
    line_count = 0
    precision = 0
    recall = 0
    fscore = 0
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
    
	line_count += 1

    	predictions = pre_line.rstrip("\n").split() 
	adopters    = ans_line.rstrip("\n").split() 
	#print predictions
	predict_count += len(predictions)
	answer_count += len(adopters)
	
	idea_correct = 0
	#sys.stdout.write("%d" % (line_count) )
	#print predictions
	#print adopters
	for i in range(len(predictions) ):
	    if predictions[i] in adopters:
		correct += 1
		idea_correct += 1
	    k_precision = float(idea_correct) / float(i + 1)
	precision += k_precision
	#sys.stdout.write( "\n")
	#print k_precision
	k_recall = float(idea_correct) / float( len(adopters))
	recall    += k_recall
	print idea_correct
	#print k_recall
	if k_precision != 0.0 and k_recall != 0.0:
	    fscore += 2.0 / ((1.0/k_precision) + (1.0/k_recall))
	''' 

    try:
        for i in range(len(plt_x)):
            plt.plot(plt_x[i], plt_y[i])

        plt.ylim(0, 1.1)
        plt.xlabel("# of data") 
        plt.ylabel("precision") 
        plt.title("The Title") 

        plt.show();
    except:
        print "no display"
	'''       

    precision /= float(line_count)
    print( "precision = %4f" %( precision ) )
    recall /= float( line_count )
    print( "recall    = %4f" %( recall ) )
    f_measure = fscore / float(line_count)
    print( "f-measure = %4f" %( f_measure ) )


