CSE532 Fall 2015 Lab3
===========================
Hang Yan: yanhangxiong@gmail.com
Wenbin Xu: seanbean1992@gmail.com

Overview
===========================
We implemented all required features from lab instructions. The project consists of two independent executables:
1. Director: (main function in director/director_cli.cpp)
   The director is one endpoint of our network application. It an extension of the previous lab. It registers itself to a producer and execute command sent from the producer. The director uses the Reactor pattern to handle events; uses Acceptor/Connector pattern to handle connections and uses Active Object pattern for concurrency. The implementation of the director consists of the following components:
   a. DirectorAgent: (director/director_agent.[h|cpp])
      This class represent the main logic of the director. It manages all other components: it initializes connection with producer, initalizes director object, registers event handlers to the reactor and runs reactors event loop.
   b. Director: (director/director.[h|cpp], director/play.[h|cpp], director/player.[h|cpp])
      This component extands the director from lab 2 to support muliple script files.
      [TODO] Wenbin Xu: fill this part
   c. DirectorAcceptor, DirectorInputHandler: (director/director_acceptor.[h|cpp])
      These two classes response to incoming connection and data stream, following Acceptor pattern.
2. Producer: (main function in producer/producer_cli.cpp)
   The producer is the other endpoint of our network application. It manages connected directors, interacts with users and sends command to directors. It uses the Reactor pattern to handle events and uses Acceptor/Connector pattern to handle connections. The implementation consists of the following components:
   a. main function (producer/producer_cli.cpp)
      The main function runs the main logic: create event handlers, initializes the listening to the designated port and interacts with users.
   b. ProducerAcceptor, ProducerInputHandler: (producer/producer_acceptor.[h|cpp])
      These two classes responses to the connection request and data transporting from directors using Acceptor pattern.
   c. LivenessChecker: (producer/liveness_checker.[h|cpp])
      This class consistantly checks the connection between all registered directors, by sending a special message and waiting for reponse. If it fails to receive response from one director, it that director will be removed from the playlist. This active connection checking ensures that users can always know the connection status of all directors. Technically, this checker is implemented as a event handler, and registered to the reactor as a timer event.
   d. PlayList: (producer/playlist.[h|cpp])
      Wrapper Facades for a list of connected director and scripts. The interface includes insert, remove and retrive a single script or a set of scripts from the same director.

There are also several components shared by both director and producer:
   a. Protocal: (common.[h|cpp])
      This static class defines the protocal between director and producer. The interface includes composing a legal command message from the command type and arguments, and parsing a command message to command type and arguments. We defines the following messages:
      (1). LIST. This message contains all scripts of a director and is sent from the director when it's registering to the producer. When receiving this message, the producer add the director and all scripts into the PlayList.
      (2). PLAY. This message is sent by the producer to instruct a director to start playing a specific script.
      (3). STOP. This message is sent by the producer to instruct a director to stop the current task.
      (4). PLAYING. This message is sent by the director once it starts playing a script. When the producer receives this message, it changes the status of this director to 'playing' and prevent users sending further PLAY command to this director.
      (5). QUIT. This message can be sent from both director and producer. The director will send this message during exiting and once received by the producer, all items from this director will be remove from the producer's playlist. The producer will send this message to all connected directors when the user instructs it to quit. After sending quit message, the producer will wait util all directors successfully quited (playlist is empty). Directors receiving this message will interrupt all ongoing work, send back QUIT message to the producer, clean state and quit.
   b. SignalHandler: (signal_handler.[h|cpp])
      This skeleton class provides global quit flags. It's also registered to the reactor as an event handler to handle Ctrl-C command. It contains two static atomic bool variables: quit_flag and interrupt_flag. When the user instruct the application to quit (either by Ctrl-C or by 'quit' command), quit_flag is set. Once neccessary cleaning is finished, interrupt_flag is set. The reactor event loop will break one interrupt_flag is set.
   c. Sender: (sender.[h|cpp])
      The skeleton class for sending data through a passed ACE_SOCK_Stream.
   d. string_util: (string_util.[h|cpp])
      Wrapper Facades for string operations.
   
