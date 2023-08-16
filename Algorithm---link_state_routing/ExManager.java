import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Scanner;

public class ExManager {
    private final String path;
    private int num_of_nodes;
    private final List<Node> nodes = new ArrayList<>();
    private final List<Thread> threads = new ArrayList<>();

    public ExManager(String path){
        this.path = path;
    }

    public Node get_node(int node_id){
        for(Node node : this.nodes){
            if(node.get_id() == node_id){
                return node;
            }
        }
        return null;
    }

    public int get_num_of_nodes() {
        return this.num_of_nodes;
    }

    public void update_edge(int id1, int id2, double weight){
        Node node1 = get_node(id1);
        Node node2 = get_node(id2);
        node1.update_edge(id2, weight);
        node2.update_edge(id1, weight);
    }

    public void read_txt(){
        try{
            File input = new File(this.path);
            Scanner scanner = new Scanner(input);
            this.num_of_nodes = Integer.parseInt(scanner.nextLine());
            while(scanner.hasNextLine()){
                String line = scanner.nextLine();
                // end of nodes information
                if(Objects.equals(line, "stop")){
                    scanner.close();
                    break;
                }
                String[] split_line = line.split(" ");

                // create a new node based on data in "line"
                int node_id = Integer.parseInt(split_line[0]);
                List<Node.Neighbor> neighbors = new ArrayList<>();
                double[][] adjacency_matrix = new double[num_of_nodes][num_of_nodes];

                for(int i=0; i < this.num_of_nodes; i++) {
                    for (int j = 0; j < this.num_of_nodes; j++) {
                        adjacency_matrix[i][j] = -1.0;
                    }
                }
                for (int i = 1; i < split_line.length; i += 4) {
                    int neighbor_id = Integer.parseInt(split_line[i]);
                    double edge_weight = Double.parseDouble(split_line[i + 1]);
                    int send_port = Integer.parseInt(split_line[i + 2]);
                    int get_port = Integer.parseInt(split_line[i + 3]);

                    // update matrix
                    adjacency_matrix[node_id - 1][neighbor_id - 1] = edge_weight;
                    adjacency_matrix[neighbor_id - 1][node_id - 1] = edge_weight;

                    // create neighbor
                    Node.Neighbor neighbor = new Node.Neighbor(neighbor_id, send_port, get_port);
                    neighbors.add(neighbor);

                }
                Node node = new Node(node_id, neighbors, adjacency_matrix);
                this.nodes.add(node);
            }
            scanner.close();
        }
        catch (IOException io){
            throw  new RuntimeException(io);
        }
    }

    public void start(){
        for (Node node : this.nodes){
            Thread thread = new Thread(node);
            this.threads.add(thread);
            thread.start();
        }
        for (Thread thread : this.threads){
            try{
                thread.join();
            }
            catch (InterruptedException ie){
                throw new RuntimeException(ie);
            }
        }
    }

    public void terminate(){

    }
}
