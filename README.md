# TCP Client in C++

This project provides a basic TCP client implementation in C++. It does not include predefined packets but allows for easy packet handling.

## Adding a Client packet
1. Create a new file inside `Packets/Client`.
2. Define a function that takes a pointer to `CallingInstance` as an argument.
3. Register it in `createMessage` by adding it to the appropriate `switch` case.

## Handling Server packets
1. Create a new file inside `Packets/Server`.
2. Define a function that takes `vector<uint8_t>` and `CallingInstance` as arguments.
3. Register it in `handlePacket` by adding it to the appropriate `switch` case.

## Structuring packet data
- Modify `Classes/CallingInstance.h` to define a new struct for packet-specific data.
- Add the struct to `CallingInstance` and set values inside the client packet.
- The server packet can then utilize this data.

## Sending packets
- Use `Messaging::createMessage(packetID, callingInstance)` to send a packet to the server.

This setup allows for easy expansion and customization of packet handling within the client-server architecture.
