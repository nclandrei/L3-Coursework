import java.io.*;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

public class CalcRun {

	public static void main(String[] args) throws Exception {
		InputStream source = new FileInputStream(args[0]);
		CalcLexer lexer = new CalcLexer(
		   new ANTLRInputStream(source));
		CommonTokenStream tokens = 
		   new CommonTokenStream(lexer);
		CalcParser parser = new CalcParser(tokens);
		ParseTree tree = parser.prog();
		ExecVisitor exec = new ExecVisitor();
		exec.visit(tree);
	}

}	 
