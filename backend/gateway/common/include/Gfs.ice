module Gfs
{
	sequence<byte> GpsRecordStream;
	sequence<GpsRecordStream> GpsRecordStreams;
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
		GpsRecordStreams records;
	};

	interface GfsProxy
	{
		// get records
		bool getRecords(Request req, out Response res);
	};

	interface GfsService
	{
		// get records
		bool getRecords(Request req, out Response res);
	};	
};

