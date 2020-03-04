module GfsOBD
{
	sequence<byte> ObdRecordStream;
	sequence<ObdRecordStream> ObdRecordStreams;
	struct Request
	{
		long uid;
		long beginTime;
		long endTime;
		int count;
	};
	struct Response
	{
		Request req;
		ObdRecordStreams records;
	};

	interface ObdGfs
	{
		// get records
		bool getRecords(Request req, out Response res);
	};	
};

