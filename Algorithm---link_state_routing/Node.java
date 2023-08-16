import java.io.*;
import java.net.*;
import java.util.*;

public class Node implements Runnable{

    private final int ID;
    private final int num_of_nodes;
    private final List<Neighbor> neighbors = new ArrayList<>();
    private final double[][] adjacency_matrix;
    private final Object lock = new Object();
    private final Map<Integer, Boolean> received_link_states = new HashMap<>();
    private final Object r_lock = new Object();
    private final int package_size_bound;

    // constructor
    public Node(int ID, List<Neighbor> neighbors, double[][] adjacency_matrix) throws IOException{
        this.ID = ID;
        this.num_of_nodes = adjacency_matrix.length;
        this.neighbors.addAll(neighbors);
        this.adjacency_matrix = adjacency_matrix;

        List<LinkState> linkStates = new ArrayList<>();
        for (int i = 0; i < this.num_of_nodes; i++) {
            linkStates.add(new LinkState(this.num_of_nodes, this.num_of_nodes, 0.0));
        }
        this.package_size_bound =  convert_to_bytes(linkStates).length;
    }

    // ---------- Node functions ----------

    public int get_id(){
        return this.ID;
    }

    public void update_edge(int node_id, double new_weight){
        synchronized (this.lock){
            this.adjacency_matrix[this.ID-1][node_id-1] = new_weight;
            this.adjacency_matrix[node_id-1][this.ID-1] = new_weight;
        }
    }

    public void print_graph(){
        for(int i=0; i<this.num_of_nodes; i++){
            for(int j=0; j<this.num_of_nodes; j++){
                System.out.print(this.adjacency_matrix[i][j]);
                if(j<this.num_of_nodes-1){
                    System.out.print(", ");
                }
                else{
                    System.out.print("\n");
                }
            }
        }
    }

    @Override
    public void run(){
        try{
            link_state_routing();
        }
        catch(InterruptedException | IOException | ClassNotFoundException e){
            throw new RuntimeException(e);
        }
    }

    private Object convert_from_bytes(byte[] bytes) throws IOException, ClassNotFoundException {
        try (ByteArrayInputStream is = new ByteArrayInputStream(bytes);
             ObjectInputStream in = new ObjectInputStream(is)) {
            return in.readObject();
        }
    }

    private byte[] convert_to_bytes(Object object) throws IOException {
        try (ByteArrayOutputStream bos = new ByteArrayOutputStream();
             ObjectOutputStream out = new ObjectOutputStream(bos)) {
            out.writeObject(object);
            out.flush();
            return bos.toByteArray();
        }
    }

    // ---------- Auxiliary classes ----------

    static class Neighbor{
        int ID;
        int send_port;
        int get_port;
        public Neighbor(int id, int send_port, int get_port){
            this.ID = id;
            this.send_port = send_port;
            this.get_port = get_port;
        }
    }

    static class LinkState implements Serializable {
        public int ID;
        public int neighbor_Id;
        public double edge_weight;

        public LinkState(int ID, int neighbor_Id, double edge_weight){
            this.ID = ID;
            this.neighbor_Id = neighbor_Id;
            this.edge_weight = edge_weight;
        }
    }

    class Receiver implements Runnable{
        private final DatagramSocket listen_port;
        private final Neighbor neighbor;

        public Receiver(Neighbor neighbor) throws SocketException{
            this.neighbor = neighbor;
            this.listen_port = new DatagramSocket(neighbor.get_port);
            this.listen_port.setSoTimeout(10000);
            this.listen_port.setBroadcast(true);
        }

        @Override
        public void run(){
            boolean updates_still_arriving = true;
            try{
                while (updates_still_arriving){
                    get_message();
                    updates_still_arriving = false;
                    for (Integer key: received_link_states.keySet()){
                        if (key != ID && !received_link_states.get(key)){
                            updates_still_arriving = true;
                        }
                    }
                }
                this.listen_port.close();
                Thread.currentThread().interrupt();
            }
            catch (IOException | ClassNotFoundException e){
                throw new RuntimeException(e);
            }
        }

        private void get_message() throws IOException, ClassNotFoundException{
            byte[] buffer = new byte[package_size_bound];
            DatagramPacket input_pocket = new DatagramPacket(buffer, buffer.length);
            try{
                this.listen_port.receive(input_pocket);
                byte[] message = new byte[input_pocket.getLength()];
                System.arraycopy(input_pocket.getData(), input_pocket.getOffset(), message, 0, input_pocket.getLength());

                List<LinkState> linkStates = (List<LinkState>) convert_from_bytes(message);
                int package_source_id = linkStates.get(0).ID;
                if(!received_link_states.get(package_source_id)){
                    for(LinkState linkState : linkStates){
                        update_edge(linkState.ID, linkState.neighbor_Id, linkState.edge_weight);
                    }
                    synchronized (r_lock){
                        received_link_states.replace(package_source_id, true);
                    }
                    DatagramSocket broadcastSocked = new DatagramSocket();
                    broadcastSocked.setBroadcast(true);
                    for (Neighbor neighbor: neighbors) {
                        if (neighbor.ID != this.neighbor.ID) {
                            Thread.sleep(10);
                            broadcastSocked.send(new DatagramPacket(message, message.length, InetAddress.getByName("127.0.0.1"), neighbor.send_port));
                        }
                    }
                    broadcastSocked.close();
                }
            }
            catch (SocketTimeoutException ignored) {}
            catch (InterruptedException | IOException | ClassNotFoundException e) {
                throw new RuntimeException(e);
            }
        }

        public void update_edge(int node_id1, int node_id2, double edge_weight){
            synchronized (lock){
                adjacency_matrix[node_id1-1][node_id2-1] = edge_weight;
            }
        }

    }

    /** Link State Routing */
    private void link_state_routing() throws IOException, ClassNotFoundException, InterruptedException{

        List<Thread> threads = new ArrayList<>();
        List<LinkState> link_states = new ArrayList<>();

        for(int node_id = 1; node_id <= this.num_of_nodes; node_id++){
            this.received_link_states.put(node_id, false);
        }

        for(Neighbor neighbor : this.neighbors){
            Thread thread = new Thread(new Receiver(neighbor));
            threads.add(thread);
            thread.start();
            // create link stated based on (new) adjacency matrix
            link_states.add(new LinkState(this.ID, neighbor.ID, this.adjacency_matrix[this.ID-1][neighbor.ID-1]));
        }
        Thread.sleep(1000);

        // convert link state to legal message (in bytes)
        byte[] message = convert_to_bytes(link_states);
        // send link states to neighbors
        DatagramSocket send_port = new DatagramSocket();
        send_port.setBroadcast(true);
        for (Neighbor neighbor: this.neighbors) {
            Thread.sleep(100);
            send_port.send(new DatagramPacket(message, message.length, InetAddress.getByName("127.0.0.1"), neighbor.send_port));

        }
        send_port.close();

        for (Thread thread: threads) {
            thread.join();
        }
    }

}