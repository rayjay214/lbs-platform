module GMQ
{
    sequence<byte> Line;
    sequence<Line> Table;
    sequence<string> StrSeq;
    
    interface MsgQueue
    {
        // Write message to every queue
        void writeMessages(["cpp:array"] Line content);
        
        // obsolete, should replaced by sequenceStringsFromTo
        bool writeMessagesTo(string queue, ["cpp:range"] Table tbl);

        // obsolete, should replaced by writeStringsToIPPort
        bool writeMessagesToIPPort(["cpp:range"] Table tbl, string caller, string calleeip, long calleeport);

        // obsolete, should replaced by sequenceStringsFromTo
        bool writeMessagesFromTo(["cpp:range"] Table tbl, string caller, string callee);
        
        // Write message to a special queue
        bool writeMessagesFrom(string queue, ["cpp:range"] Table tbl, string caller);

        // Write message to every queue
        void writeString(string str);
        
        // Write message to a special queue
        bool writeStringsTo(string queue, StrSeq strs);

        // Read message from a special queue
        long readMessages(string queue, out Table tbl);

        // Read message from a special queue
        long readStrings(string queue, out StrSeq strs);

        // Ack to received message, server will remove it after that
        void readMessagesAck(string queue, long seq);
        
        // Maintenance queue
        bool addQueue(string queue);
        // If there is a replication, then do not remove it.
        bool removeQueue(string queue);
        void listQueue(out StrSeq queues);
        
        // Maintenance replication of queue, 
        // Ensure the queue is added before creation replication
        bool addRepQueue(string queue, StrSeq repQueues);
        bool removeRepQueue(string queue, StrSeq repQueues);
        void listRepQueue(string queue, out StrSeq repQueues);
        
        // Get/Set gmq status
        void getGmqStatus(out StrSeq status);
        void setGmqStatus(StrSeq status);
        void getGmqVerboseStatus(out StrSeq status);

        // Write message to a special callee,  notice it can only called by c++ program. for java, is waiting for another function
        // return 0 means success.
        long writeStringsFromTo(["cpp:range"]StrSeq strs, string caller, string callee, long shareid, long version);

        //the same as writeStringsFromTo,except that it whill try to connect previous callee
        // return 0 means success.
        long sequenceStringsFromTo(["cpp:range"]StrSeq strs, string caller, string callee, long shareid, long version, 
        		string incalleeip, long incalleeport, out string outcalleeip, out long outcalleeport);

        // Write message to a special callee,  notice it can only called by c++ program. for java, is waiting for another function
        // return 0 means success.
        long writeStringsToIPPort(["cpp:range"]StrSeq strs, string caller, string callee, string ip, long port);

        //should only be called by gmq router
        // return 0 means success.
        long routeStringsFromTo(["cpp:range"]StrSeq strs, string caller, string callee, long shareid, long version, 
        		string incalleeip, long incalleeport, string calleeidc, long routestep);
    };
};

