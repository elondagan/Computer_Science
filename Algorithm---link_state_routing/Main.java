import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws FileNotFoundException {
        String[] paths = {"D:\\Desktop\\DS\\DS_Project\\src\\inputs\\input_1.txt",
                "D:\\Desktop\\DS\\DS_Project\\src\\inputs\\input_2.txt",
                "D:\\Desktop\\DS\\DS_Project\\src\\inputs\\input_3.txt",
                "D:\\Desktop\\DS\\DS_Project\\src\\inputs\\input_4.txt",
                "D:\\Desktop\\DS\\DS_Project\\src\\inputs\\input_5.txt"}; //enter the path to the files you want to run here.
        for(String path: paths) {
            ExManager m = new ExManager(path);
            m.read_txt();


            int num_of_nodes = m.get_num_of_nodes();

            Scanner scanner = new Scanner(new File(path));
            while(scanner.hasNextLine()){
                String line = scanner.nextLine();
                if(line.contains("start")){
                    m.start();
                    Node n = m.get_node(1 + (int)(Math.random() * num_of_nodes));
                    n.print_graph();
                    System.out.println();
                }

                if(line.contains("update")){
                    String[] data = line.split(" ");
                    m.update_edge(Integer.parseInt(data[1]), Integer.parseInt(data[2]), Double.parseDouble(data[3]));
                }
            }
            m.terminate();
        }
    }
}
