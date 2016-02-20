// Generated from Fun.g4 by ANTLR 4.5.1
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link FunParser}.
 */
public interface FunListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by the {@code prog}
	 * labeled alternative in {@link FunParser#program}.
	 * @param ctx the parse tree
	 */
	void enterProg(FunParser.ProgContext ctx);
	/**
	 * Exit a parse tree produced by the {@code prog}
	 * labeled alternative in {@link FunParser#program}.
	 * @param ctx the parse tree
	 */
	void exitProg(FunParser.ProgContext ctx);
	/**
	 * Enter a parse tree produced by the {@code proc}
	 * labeled alternative in {@link FunParser#proc_decl}.
	 * @param ctx the parse tree
	 */
	void enterProc(FunParser.ProcContext ctx);
	/**
	 * Exit a parse tree produced by the {@code proc}
	 * labeled alternative in {@link FunParser#proc_decl}.
	 * @param ctx the parse tree
	 */
	void exitProc(FunParser.ProcContext ctx);
	/**
	 * Enter a parse tree produced by the {@code func}
	 * labeled alternative in {@link FunParser#proc_decl}.
	 * @param ctx the parse tree
	 */
	void enterFunc(FunParser.FuncContext ctx);
	/**
	 * Exit a parse tree produced by the {@code func}
	 * labeled alternative in {@link FunParser#proc_decl}.
	 * @param ctx the parse tree
	 */
	void exitFunc(FunParser.FuncContext ctx);
	/**
	 * Enter a parse tree produced by the {@code formal}
	 * labeled alternative in {@link FunParser#formal_decl}.
	 * @param ctx the parse tree
	 */
	void enterFormal(FunParser.FormalContext ctx);
	/**
	 * Exit a parse tree produced by the {@code formal}
	 * labeled alternative in {@link FunParser#formal_decl}.
	 * @param ctx the parse tree
	 */
	void exitFormal(FunParser.FormalContext ctx);
	/**
	 * Enter a parse tree produced by the {@code var}
	 * labeled alternative in {@link FunParser#var_decl}.
	 * @param ctx the parse tree
	 */
	void enterVar(FunParser.VarContext ctx);
	/**
	 * Exit a parse tree produced by the {@code var}
	 * labeled alternative in {@link FunParser#var_decl}.
	 * @param ctx the parse tree
	 */
	void exitVar(FunParser.VarContext ctx);
	/**
	 * Enter a parse tree produced by the {@code bool}
	 * labeled alternative in {@link FunParser#type}.
	 * @param ctx the parse tree
	 */
	void enterBool(FunParser.BoolContext ctx);
	/**
	 * Exit a parse tree produced by the {@code bool}
	 * labeled alternative in {@link FunParser#type}.
	 * @param ctx the parse tree
	 */
	void exitBool(FunParser.BoolContext ctx);
	/**
	 * Enter a parse tree produced by the {@code int}
	 * labeled alternative in {@link FunParser#type}.
	 * @param ctx the parse tree
	 */
	void enterInt(FunParser.IntContext ctx);
	/**
	 * Exit a parse tree produced by the {@code int}
	 * labeled alternative in {@link FunParser#type}.
	 * @param ctx the parse tree
	 */
	void exitInt(FunParser.IntContext ctx);
	/**
	 * Enter a parse tree produced by the {@code assn}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void enterAssn(FunParser.AssnContext ctx);
	/**
	 * Exit a parse tree produced by the {@code assn}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void exitAssn(FunParser.AssnContext ctx);
	/**
	 * Enter a parse tree produced by the {@code proccall}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void enterProccall(FunParser.ProccallContext ctx);
	/**
	 * Exit a parse tree produced by the {@code proccall}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void exitProccall(FunParser.ProccallContext ctx);
	/**
	 * Enter a parse tree produced by the {@code if}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void enterIf(FunParser.IfContext ctx);
	/**
	 * Exit a parse tree produced by the {@code if}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void exitIf(FunParser.IfContext ctx);
	/**
	 * Enter a parse tree produced by the {@code while}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void enterWhile(FunParser.WhileContext ctx);
	/**
	 * Exit a parse tree produced by the {@code while}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void exitWhile(FunParser.WhileContext ctx);
	/**
	 * Enter a parse tree produced by the {@code switch}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void enterSwitch(FunParser.SwitchContext ctx);
	/**
	 * Exit a parse tree produced by the {@code switch}
	 * labeled alternative in {@link FunParser#com}.
	 * @param ctx the parse tree
	 */
	void exitSwitch(FunParser.SwitchContext ctx);
	/**
	 * Enter a parse tree produced by {@link FunParser#fun_case}.
	 * @param ctx the parse tree
	 */
	void enterFun_case(FunParser.Fun_caseContext ctx);
	/**
	 * Exit a parse tree produced by {@link FunParser#fun_case}.
	 * @param ctx the parse tree
	 */
	void exitFun_case(FunParser.Fun_caseContext ctx);
	/**
	 * Enter a parse tree produced by {@link FunParser#fun_default}.
	 * @param ctx the parse tree
	 */
	void enterFun_default(FunParser.Fun_defaultContext ctx);
	/**
	 * Exit a parse tree produced by {@link FunParser#fun_default}.
	 * @param ctx the parse tree
	 */
	void exitFun_default(FunParser.Fun_defaultContext ctx);
	/**
	 * Enter a parse tree produced by the {@code seq}
	 * labeled alternative in {@link FunParser#seq_com}.
	 * @param ctx the parse tree
	 */
	void enterSeq(FunParser.SeqContext ctx);
	/**
	 * Exit a parse tree produced by the {@code seq}
	 * labeled alternative in {@link FunParser#seq_com}.
	 * @param ctx the parse tree
	 */
	void exitSeq(FunParser.SeqContext ctx);
	/**
	 * Enter a parse tree produced by {@link FunParser#expr}.
	 * @param ctx the parse tree
	 */
	void enterExpr(FunParser.ExprContext ctx);
	/**
	 * Exit a parse tree produced by {@link FunParser#expr}.
	 * @param ctx the parse tree
	 */
	void exitExpr(FunParser.ExprContext ctx);
	/**
	 * Enter a parse tree produced by {@link FunParser#sec_expr}.
	 * @param ctx the parse tree
	 */
	void enterSec_expr(FunParser.Sec_exprContext ctx);
	/**
	 * Exit a parse tree produced by {@link FunParser#sec_expr}.
	 * @param ctx the parse tree
	 */
	void exitSec_expr(FunParser.Sec_exprContext ctx);
	/**
	 * Enter a parse tree produced by the {@code false}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterFalse(FunParser.FalseContext ctx);
	/**
	 * Exit a parse tree produced by the {@code false}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitFalse(FunParser.FalseContext ctx);
	/**
	 * Enter a parse tree produced by the {@code true}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterTrue(FunParser.TrueContext ctx);
	/**
	 * Exit a parse tree produced by the {@code true}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitTrue(FunParser.TrueContext ctx);
	/**
	 * Enter a parse tree produced by the {@code num}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterNum(FunParser.NumContext ctx);
	/**
	 * Exit a parse tree produced by the {@code num}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitNum(FunParser.NumContext ctx);
	/**
	 * Enter a parse tree produced by the {@code id}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterId(FunParser.IdContext ctx);
	/**
	 * Exit a parse tree produced by the {@code id}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitId(FunParser.IdContext ctx);
	/**
	 * Enter a parse tree produced by the {@code funccall}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterFunccall(FunParser.FunccallContext ctx);
	/**
	 * Exit a parse tree produced by the {@code funccall}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitFunccall(FunParser.FunccallContext ctx);
	/**
	 * Enter a parse tree produced by the {@code not}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterNot(FunParser.NotContext ctx);
	/**
	 * Exit a parse tree produced by the {@code not}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitNot(FunParser.NotContext ctx);
	/**
	 * Enter a parse tree produced by the {@code parens}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void enterParens(FunParser.ParensContext ctx);
	/**
	 * Exit a parse tree produced by the {@code parens}
	 * labeled alternative in {@link FunParser#prim_expr}.
	 * @param ctx the parse tree
	 */
	void exitParens(FunParser.ParensContext ctx);
	/**
	 * Enter a parse tree produced by {@link FunParser#actual}.
	 * @param ctx the parse tree
	 */
	void enterActual(FunParser.ActualContext ctx);
	/**
	 * Exit a parse tree produced by {@link FunParser#actual}.
	 * @param ctx the parse tree
	 */
	void exitActual(FunParser.ActualContext ctx);
}