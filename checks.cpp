// coomands.h : DefaultIO

/*void upload_file(string path, string name_file){
		ifstream train_file;
		fstream myFile;
		myFile.open(name_file);
		train_file.open(path);
		string line;
		while (getline(train_file, line) && line != "done") /////////////add!
		{
			myFile << line;
		}
		myFile.close();
		train_file.close();
		TimeSeries t = TimeSeries("anomalyTrain.csv"); ///// detlete!!
		//this->read();

	}*/


// coomands.h : first command

    	/*
		dio->write(this->description);
		string address_csv = dio->read();
		dio->upload_file(address_csv, "anomalyTrain.csv");
		info.ts_train = new TimeSeries("anomalyTrain.csv"); ///// detlete!!
		dio->write("Upload complete.");

		dio->write(this->second_description);
		address_csv = dio->read();
		dio->upload_file(address_csv, "anomalyTest.csv");
		info.ts_test = new TimeSeries("anomalyTest.csv"); // delete!!
		dio->write("Upload complete.");
		*/


// coomands.h : upload

    /*string address_csv = dio->read();
		dio->upload_file(address_csv, "anomalies.csv");
		dio->write("Done");

		//make 
		ifstream anomalies_file;
		anomalies_file.open(address_csv);
		string line, first_word, second_word;
		vector<pair<int,int>> anomalies_vec;
		int sum_of_exseptions = 0;
		while (getline(anomalies_file, line) && line != "done") /////////////add!
		{
			stringstream t(line);
			while (getline(t, first_word, ','))
			while (getline(t, second_word, ','))
			{
			anomalies_vec.push_back(make_pair(stoi(first_word),stoi(second_word)));
			sum_of_exseptions +=stoi(second_word) - stoi(first_word) +1;
			}
		}
		anomalies_file.close();*/