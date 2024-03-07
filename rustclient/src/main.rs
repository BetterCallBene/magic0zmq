mod async_helpers;

use std::{error::Error, rc};
use zeromq::{Socket, SocketRecv, SocketSend};
use serde::{Serialize, Deserialize};
use rmp_serde::{Deserializer, Serializer};

#[derive(Serialize, Deserialize, Debug)]
struct OutGoingMessage {
    #[serde(rename = "Name")]
    name: String,
}

impl OutGoingMessage {
    fn new(name: String) -> Self {
        OutGoingMessage { name }
    }
}

struct IncomingMessage {
    name: String,
}


#[async_helpers::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let mut socket = zeromq::ReqSocket::new();
    socket.connect("tcp://172.17.0.2:5555")
        .await
        .expect("Failed to connect to server");

    println!("Connected to server");

    let outgoing_message = OutGoingMessage::new("Rust".to_string());

    let mut buf = Vec::new();
    outgoing_message.serialize(&mut Serializer::new(&mut buf))
        .expect("Failed to serialize outgoing message");

    socket.send(buf.into())
        .await
        .expect("Failed to send message");

    Ok(())
}
