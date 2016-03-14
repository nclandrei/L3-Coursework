import java.sql.*;

/**
 * GUID: 2147392n
 * DB(H) - Assessed Exercise 2
 */
public class Lev3152147392n {

    /**
     * Provides the average rating of releases with a rating >=8
     * @param statement the statement that will be used to execute the query
     */
    public static void Method3A(Statement statement) {
        String query = "SELECT AVG(RATING) AS AVERAGE FROM RELEASE WHERE RATING >= 8";
        try {
            ResultSet results = statement.executeQuery(query);
            while (results.next()) {
                System.out.println(results.getString("AVERAGE") + "\n");
            }
            System.out.print("\n");
        }
        catch (SQLException e) {
            System.out.println("Failed to execute query for Method3A!\n");
            e.printStackTrace();
            return;
        }
    }

    /**
     * Provides the musicians' names that are still alive, are actively involved in more than one band
     * and perform at more than one instrument
     * @param statement the statement that will be used to execute the query
     */   
     public static void Method3B(Statement statement) {
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
            System.out.print("\n");
        }
        catch (SQLException e) {
            System.out.println("Failed to execute query for Method3B!\n");
            e.printStackTrace();
            return;
        }
    }

    /**
     * Provides the band name(s) that have the maximum number of current members among all bands
     * @param statement the statement that will be used to execute the query
     */    
     public static void Method3C(Statement statement) {
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
            System.out.print("\n");
        }
        catch (SQLException e) {
            System.out.println("Failed to execute query for Method3C\n");
            e.printStackTrace();
            return;
        }
    }

    public static void main(String[] args) {

        try {
            Class.forName("org.postgresql.Driver");
        }
        catch (ClassNotFoundException e) {
            System.out.println("Could not find JDBC Driver");
            e.printStackTrace();
            return;
        }

        Connection connection;
        try {
            connection =
                    DriverManager.getConnection("jdbc:postgresql://yacata.dcs.gla.ac.uk:5432/",
                            "lev3_15_2147392n", "2147392n");
        }
        catch (SQLException e) {
            System.out.println("Connection Failed!");
            e.printStackTrace();
            return;
        }

        if (connection == null) {
            System.out.println("Failed to establish connection!");
        }

        Statement statement;
        try {
            statement = connection.createStatement();
        }
        catch (SQLException e) {
            System.out.println("Failed to create statement!\n");
            e.printStackTrace();
            return;
        }

        assert connection != null;

        assert statement != null;

        // running first SQL query
        System.out.println("Results for Method3A: \n");
            Method3A(statement);

        // running second SQL query
        System.out.println("Results for Method3B: \n");
        Method3B(statement);

        //running third SQL query
        System.out.println("Results for Method3C: \n");
        Method3C(statement);


        try {
            statement.close();
        }
        catch (SQLException e) {
            System.out.println("Failed to close statement!\n");
            e.printStackTrace();
            return;
        }

        try {
            connection.close();
        }
        catch (SQLException e) {
            System.out.println("Failed to close the connection!\n");
            e.printStackTrace();
            return;
        }
    }
}
