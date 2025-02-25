# TCP Client in C++

This project provides a basic TCP client implementation in C++. It does not include predefined packets but allows for easy packet handling.

## Adding a Client Packet
1. Create a new file inside `Packets/Client`.
2. Define a function that takes a pointer to `CallingInstance` and a socket as arguments.
3. Register the function inside `Classes/Core/Messaging.cpp` by modifying `handlePacket`.
4. Use a `switch` statement to call the function based on the received packet ID.

## Creating a Server Packet
1. Create a new file inside `Packets/Server`.
2. Define a function that takes `CallingInstance` as an argument.
3. Register it in `createMessage` by adding it to the appropriate `switch` case.

## Structuring Packet Data
- Modify `Classes/CallingInstance.h` to define a new struct for packet-specific data.
- Add the struct to `CallingInstance` and set values inside the client packet.
- The server packet can then utilize this data.

## Sending Packets
- Use `Messaging::createMessage(packetId, callingInstance)` to send a packet to the server.

This setup allows for easy expansion and customization of packet handling within the client-server architecture.
