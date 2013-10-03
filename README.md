CMU 15-441: Computer Networks
Project 1: A Web Server Called Liso
Wei Shen | wshen1@andrew.cmu.edu
===

Lisod v1.0

Usage: ./lisod < HTTP port > < HTTPS port > < log ﬁle > < lock ﬁle > < www folder > < CGI folder or script name > < private key ﬁle > < certiﬁcate ﬁle >

 - HTTP port – the port for the HTTP (or echo) server to listen on
 - HTTPS port – the port for the HTTPS server to listen on
 - log ﬁle – ﬁle to send log messages to (debug, info, error)
 - lock ﬁle – ﬁle to lock on when becoming a daemon process
 - www folder – folder containing a tree to serve as the root of a website
 - CGI folder or script name – folder containing CGI programs—each ﬁle should
   be executable;
 - if a ﬁle it should be a script where you redirect all /cgi/* URIs to
 - private key ﬁle – private key ﬁle path
 - certiﬁcate ﬁle – certiﬁcate ﬁle path
