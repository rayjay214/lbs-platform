module GMQDirect
{
    sequence<byte> Line;
    sequence<Line> Table;
    sequence<string> StrSeq;

    interface GMQDirectSend
    {
        // obsolete, should replaced by writeStringsFromTo
        bool writeMessages( ["cpp:range"] Table tbl);

        // obsolete, should replaced by writeStringsFromTo
        bool writeMessagesToIPPort(["cpp:range"] Table tbl, string caller, string calleeip, long calleeport);

        // obsolete, should replaced by writeStringsFromTo
        bool writeMessagesFromTo(["cpp:range"] Table tbl, string caller, string callee);
        
        // obsolete, should replaced by writeStringsFromTo
        bool writeMessagesFrom(string queue, ["cpp:range"] Table tbl, string caller);

        // Write message to a special callee,  notice it can only called by c++ program. for java, is waiting for another function
        // return 0 means success.
        long writeStringsFromTo(["cpp:range"]StrSeq strs, string caller, string callee, long shareid, long version);

        // obsolete, because support gmq route, writeStringsToIPPort is also send by writeStringsFromTo.
        // return 0 means success.
        ["ami"] long writeStringsToIPPort(["cpp:range"]StrSeq strs, string caller, string callee, string calleeip, long calleeport);
    };
};

