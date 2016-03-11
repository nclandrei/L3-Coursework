import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

public class MainClass {
    public static void main(String[] args) {
        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("Could not fine JDBC Driver");
            e.printStackTrace();
            return;
        }
        Connection connection = null;
        try {
            connection =
                    DriverManager.getConnection("jdbc:postgresql://yacata.dcs.gla.ac.uk:5432/",
                            "lev3_15_2147392n", "Niko1994");
        } catch (SQLException e) {
            System.out.println("Connection Failed!");
            e.printStackTrace();
            return;
        }
        if (connection != null) {
            System.out.println("Controlling your database...");
        } else {
            System.out.println("Failed to establish connection!");
        }
        try {
            Statement statement = connection.createStatement();
            String sqlString = "CREATE TABLE ToBeDeleted (name integer)";
            statement.executeUpdate(sqlString);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
