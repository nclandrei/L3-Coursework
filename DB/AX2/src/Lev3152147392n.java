import java.sql.*;

public class Lev3152147392n {

    public static void method3A(Statement statement) {
        String query = "SELECT AVG(RATING) AS AVERAGE FROM RELEASE WHERE RATING >= 8";
        try {
            ResultSet results = statement.executeQuery(query);
            while (results.next()) {
                System.out.println(results.getString("AVERAGE") + "\n");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void method3B(Statement statement) {
        String query = "SELECT name " +
                "FROM member " +
                "JOIN memberof ON member.mid = memberof.mid " +
                "WHERE stillalive = 'Y' AND endyear IS NULL " +
                "GROUP BY name " +
                "HAVING count(DISTINCT instrument) > 1 AND count(DISTINCT memberof.bid) > 1";
        try {
            ResultSet results = statement.executeQuery(query);
            while (results.next()) {
                System.out.println(results.getString("name") + "\n");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void method3C(Statement statement) {
        String query = "SELECT name, count(DISTINCT memberof.mid) AS count_current_members " +
                "FROM band " +
                "JOIN memberof ON band.bid = memberof.bid " +
                "WHERE endyear IS NULL " +
                "GROUP BY name " +
                "HAVING count(DISTINCT memberof.mid) = (SELECT max(cnt) " +
                "FROM (SELECT DISTINCT bid, count (mid) AS cnt " +
                "FROM memberof " +
                "WHERE endyear IS NULL " +
                "GROUP BY bid) c " +
                ");";
        try {
            ResultSet results = statement.executeQuery(query);
            while (results.next()) {
                System.out.println(results.getString("name") + "\t" + results.getInt("count_current_members"));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

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
                    DriverManager.getConnection("jdbc:postgresql://localhost:5432/",
                            "postgres", "CoriNico9594GLA");
        } catch (SQLException e) {
            System.out.println("Connection Failed!");
            e.printStackTrace();
            return;
        }
        if (connection != null) {
            System.out.println("Connection successful\n");
        } else {
            System.out.println("Failed to establish connection!");
        }
        try {
            assert connection != null;
            Statement statement = connection.createStatement();

            // running first SQL query
            System.out.println("Results for Method3A\n");
            method3A(statement);

            // running second SQL query
            System.out.println("Results for Method3B\n");
            method3B(statement);

            //running third SQL query
            System.out.println("Results for Method3C\n");
            method3C(statement);

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
