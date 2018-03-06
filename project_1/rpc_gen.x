

program COMMUNICATE_PROG {
      version COMMUNICATE_VERSION
      {
        int JoinServer(string IP, int ProgID, int ProgVers) = 1;
        int LeaveServer (string IP, int ProgID, int ProgVers) = 2;
        int Join (string IP, int Port) = 3;
        
        int Leave (string IP, int Port) = 4;
        int Subscribe(string IP, int Port, string Article) = 5;
        int Unsubscribe (string IP, int Port, string Article) = 6;
        int Publish (string Article, string IP, int Port) = 7;
        int PublishServer (string Article, string IP, int Port) = 8;
        int Ping () = 9;
       } = 1;
} = 0x123123;
