****This is a simple file server and client software demo. It is a demo of how I set up a simple server and client in C with a GUI for the client in C++. The client can log in or create an account and the details of which are sent to the server which is then stored in a database. Once logged in the Client and Download and Upload files to the server by clicking on the file they would like to interact with.**** 

## **Features:**

1. Send and recieve files easily between a client and the server.
2. Simple setup from the client end to send and recieve
3. A simple GUI interface to allow the user to choose a file they would to install from the server
4. Terminal text interface for use on the server.
5. Logs on both sides to keep track of usage.
6. Account feature, which allows the client to create an account and the details are stored on a database. Then the client can log on using those details to access the server.
7. Password Security using Hashing. When the username and password are stored on the database, they are first sent to the server. Then the sever hashes the password before storing the details on the database which uses SQLite.

## **Future Features:**

1. Add mutlithreading so multiple clients can connect to the server.
2. Add unique spaces so each client has there own space on the server.
3. Be able to set up a server on one computer and be able to connect to the server on another computer and interact with it.


## **How to run:**
1. Create a folder which will serve as the location of the files for the server.
2. Create a folder which will be the location where you can upload files to the server.
3. Move the folder called Server out of the Bartmoss cloud files
4. Then run the cmake files for both to get it up and running. NOTE: The server needs to be made first for this to work. 

   
