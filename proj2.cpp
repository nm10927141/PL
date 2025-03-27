# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <string>
# include <vector>
# include <map>

using namespace std;

string TO_STRING( const int num ) ;
string TO_STRING( const double num ) ;

int   STOI( const string & str ) ;
float STOF( const string & str ) ;

bool CHAR_IS_SYMBOL( const char & c ) ;
bool CHAR_IS_NUMBER( const char & c ) ;

bool IS_FLOAT( const string & str ) ;
bool IS_WHITESPACE( const char & c ) ;
void DEBUG( string str ) ;
void DEBUGY( string str ) ;

# define to_string TO_STRING

# define stoi STOI
# define stof STOF

# define char_is_symbol CHAR_IS_SYMBOL
# define char_is_number CHAR_IS_NUMBER

# define is_float       IS_FLOAT
# define is_whitespace  IS_WHITESPACE

# define debug  DEBUG
# define debugY DEBUGY

enum DataType {
  NONE, 
  ERROR, 

  DATATYPE, 
  CONSTANT, 
  ELSE, 
  RETURN, 
  CONDITIONAL_CONTROLFLOW, 
  OPERATOR, 
  LEFT_PARENTHESE, 
  RIGHT_PARENTHESE, 
  SYMBOL, 
  FUNCTION, 
  SUBROUTINE, 

  VOID, 
  BOOLEAN, 
  INT, 
  FLOAT, 
  STRING, 
  CHAR, 
  OBJECT, 
  TUPLE, 
  ARRAY
} ;

enum OperationMode {
  OPERATION, 
  ERROR_OPERATION, 
  DECLARATION, 
  FUNCTION_DECLARATION, 
  VARIABLE_DECLARATION, 
  CONTROLFLOW_OPERATION
} ;

DataType STRING_TO_DATATYPE( string str ) ;
# define string_to_dataType STRING_TO_DATATYPE

# define operation moperation

# define str mstr
# define priority mpriority
# define operands moperands
# define rightass mrightass
# define resovable mresovable

# define type mtype
# define inum minum
# define fnum mfnum
# define op mop

# define data mdata
# define exp mexp

# define operations moperations
# define variables mvariables

# define name mname
# define dataType mdataType
# define params mparams
# define routineName mroutineName
# define showup mshowup
# define showlist mshowlist


# define Operators   mOperators
# define Keywords    mKeywords
# define Routines    mRoutines
# define Functions   mFunctions
# define subroutines msubroutines

# define ch mch
# define code mcode
# define index mindex
# define len mlen
# define line_reset mline_reset
# define line mline
# define pushed mpushed
# define delayed mdelayed
# define controlflow_delayed mcontrolflow_delayed
# define token_delayed mtoken_delayed
# define token mtoken
# define prevToken mprevToken
# define prevData mprevData
# define opstack mopstack
# define subject msubject
# define parentheses mparentheses
# define routineID mroutineID
# define tokens mtokens
# define tokens_layer mtokens_layer
# define var mvar
# define is_reference mis_reference

# define program mprogram
# define parser mparser

class Operator {
public : 
  string str;
  int    priority;
  int    operands;
  bool   rightass; // default are left to right associative, else are inversed
  string resovable;

  Operator() {
    str = "";
    priority = - 1;
    operands = 2;
    rightass = false;
  } // Operator()


  Operator( string s, int p ) {
    str = s;
    priority = p;
    operands = 2;
    rightass = false;
    resovable = "11";
  } // Operator()


  Operator( string s, int p, int ops, bool right_associative, string resovable_case ) {
    str = s;
    priority = p;
    operands = ops;
    rightass = right_associative;
    resovable = resovable_case;
  } // Operator()

} ;

class Token {
public : 
  DataType type;
  string str;
  int inum;
  float fnum;
  Operator op;

  Token() {
    Reset() ;
  } // Token()


  Token( int num, DataType dataType ) {
    inum = fnum = num;
    type = dataType;
  } // Token()


  Token( float num, DataType dataType ) {
    inum = fnum = num;
    type = dataType;
  } // Token()


  Token( string s, DataType dataType ) {
    str  = s;
    type = dataType;

    if ( dataType == INT ) 
      fnum = inum = stoi( s ) ;
    if ( dataType == FLOAT ) 
      fnum = inum = stof( s ) ;
    if ( dataType == BOOLEAN ) 
      fnum = inum = s == "true" ? 1 : 0;
  } // Token()


  Token( float num ) {
    inum = fnum = num;
    type = FLOAT;
  } // Token()


  Token( int num ) {
    inum = fnum = num;
    type = INT;
  } // Token()


  Token( bool num ) {
    inum = fnum = num;
    type = BOOLEAN;
  } // Token()


  Token( Operator & oper ) {
    op = oper;
    str = op.str;
    type = OPERATOR;
  } // Token()


  void Print() {
    if ( type == BOOLEAN ) 
      cout << ( inum == 1 ? "true" : "false" ) ;

    else if ( type == INT ) 
      cout << inum;

    else if ( type == FLOAT ) 
      cout << fixed << setprecision( 3 ) << fnum;
    
    else 
      cout << str;
  } // Print()


  void Reset() {
    type = NONE;
    str = "";
    fnum = inum = 0;
    op = Operator() ;
  } // Reset()


  bool IsOperand() {
    return type == SYMBOL || type == BOOLEAN || type == INT || type == FLOAT || 
        type == STRING || type == CHAR || type == OBJECT;
  } // IsOperand()


  string GetString() {
    string result;
    if ( type == BOOLEAN ) 
      result = ( inum == 1 ? "true" : "false" ) ;

    else if ( type == INT ) 
      result = to_string( inum ) ;

    else if ( type == FLOAT ) 
      result = to_string( fnum ) ;
    
    else 
      result = "'" + str + "'";

    result += "[";
    result += type == ERROR       ? "error" : 
         type == DATATYPE    ? "datatype" : 
         type == ELSE        ? "else" : 
         type == OPERATOR    ? "operator" : 
         type == SYMBOL      ? "symbol" : 
         type == FUNCTION    ? "function" : 
         type == SUBROUTINE  ? "subroutine" : 
         type == TUPLE       ? "tuple" : 
         type == BOOLEAN     ? "bool" : 
         type == INT         ? "int" : 
         type == FLOAT       ? "float" : 
         type == STRING      ? "string" : 
         type == CHAR        ? "char"  : 
         type == OBJECT      ? "object" : 
         type == VOID        ? "void" : 
         type == CONDITIONAL_CONTROLFLOW ? "cond-ctrl" : "";
    result += "," + to_string( type ) + "]";
    return result;
  } // GetString()

} ;

 // Handle of Token & Data processing
class Data {
public : 
  vector < Token > data;
  DataType type;

  Data() {
    Reset() ;
  } // Data()


  Data( Token & token ) {
    data.push_back( token ) ;
    type = token.type;
  } // Data()


  Data( string s, DataType dataType ) {
    Token temp = Token( s, dataType ) ;
    data.push_back( temp ) ;
    type = dataType;
  } // Data()


  Data( DataType dataType ) {
    type = dataType;
  } // Data()


  Token GetToken() {
    return data.empty() ? Token() : data.back() ;
  } // GetToken()


  void Reset() {
    data.clear() ;
    type = NONE;
  } // Reset()


  bool IsOperand() {
    return type == SYMBOL || type == BOOLEAN || type == INT || type == FLOAT || 
        type == STRING || type == CHAR || type == OBJECT;
  } // IsOperand()


  void Print() {
    data.back() .Print() ;
  } // Print()


  string GetString() {
    string result;
    if ( type == TUPLE || type == ARRAY ) {
      result += "(";
      for ( int k = 0 ; k < data.size() ; k ++ ) {
        result += data[k].GetString() ;
      } // for

      result += ")[";
      result += type == TUPLE ? "tuple" : "array";
      result += "]";
      return result;
    } // if

    else
      return GetToken() .GetString() ;
  } // GetString()

} ;

class Operation {
public : 
  OperationMode mode;
  vector < Data > exp;

  Operation() {
    Reset() ;
  } // Operation()


  void Reset() {
    mode = OPERATION;
    exp.clear() ;
  } // Reset()


  void DebugPrint() {
    string str = "{ ";
    vector < Data > :: iterator it;
    for ( it = exp.begin() ; it != exp.end() ; it ++ ) {
      str += it -> GetString() ;
      str += " ";
    } // for

    str += "}\n";
    debug( str ) ;
  } // DebugPrint()

} ;

# define TokenVector gTokenVector
# define DataVector  gDataVector
# define emptyData   gemptyData

class Routine {
public : 
  vector < Operation > operations;
  map < string, Data > variables;

  Routine() {
    Reset() ;
  } // Routine()


  void Execute() {
    for ( int i = 0 ; i < operations.size() ; i ++ ) {
      operations[i].DebugPrint() ;
    } // for

  } // Execute()


  void Reset() {
    operations.clear() ;
    variables.clear() ;
  } // Reset()

} ;

class Parameter {
public : 
  string var;
  bool is_reference;

  Parameter() {
    var = "";
    is_reference = false;
  } // Parameter()


  Parameter( string pv, bool ir ) {
    var = pv;
    is_reference = ir;
  } // Parameter()

} ;

class Function {
public : 
  string name;
  DataType dataType;
  vector < Parameter > params;
  string routineName;
  string showup;
  bool showlist;

  Function() {
    name = "";
    dataType = VOID;
    routineName = "";
    showup = "";
    showlist = false;
  } // Function()


  Function( string func_name, DataType dt ) {
    name = func_name;
    dataType = dt;
    params.clear() ;
    routineName = "";
    showup = "";
    showlist = false;
  } // Function()


  Function( string func_name, DataType dt, vector < Parameter > & parameters ) {
    name = func_name;
    dataType = dt;
    params = parameters;
    routineName = "";
    showup = "";
    showlist = false;
  } // Function()


  Function( string f, DataType d, vector < Parameter > & p, string fn, string s ) {
    name = f;
    dataType = d;
    params = p;
    routineName = fn;
    showup = s;
    showlist = true;
  } // Function()

} ;

class Program {
public : 
  map < string, Operator > Operators;
  map < string, Data >     Keywords;
  map < string, Routine >  Routines;
  map < string, Function > Functions;

  vector < Routine > subroutines;

  Program() {
    Reset() ;
  } // Program()


  void TableSetup() {
    Operators[ "++"] = Operator( "++", 2, 1, false, "1" ) ;
    Operators[ "--"] = Operator( "--", 2, 1, false, "1" ) ;

    Operators[ "!"]  = Operator( "!",  3, 1, true, "1" ) ;

    Operators[ "*"]  = Operator( "*",  5 ) ;
    Operators[ "/"]  = Operator( "/",  5 ) ;
    Operators[ "%"]  = Operator( "%",  5 ) ;

    Operators[ "+"]  = Operator( "+",  6 ) ;
    Operators[ "-"]  = Operator( "-",  6 ) ;

    Operators[ "<<"] = Operator( "<<", 7 ) ;
    Operators[ ">>"] = Operator( ">>", 7 ) ;

    Operators[ ">"]  = Operator( ">",  9 ) ;
    Operators[ "<"]  = Operator( "<",  9 ) ;
    Operators[ ">="] = Operator( ">=", 9 ) ;
    Operators[ "<="] = Operator( "<=", 9 ) ;

    Operators[ "=="] = Operator( "==", 10 ) ;
    Operators[ "!="] = Operator( "!=", 10 ) ;

    Operators[ "&"] = Operator( "&", 11 ) ;
    Operators[ "^"] = Operator( "^", 12 ) ;
    Operators[ "|"] = Operator( "|", 13 ) ;

    Operators[ "&&"] = Operator( "&&", 14 ) ;

    Operators[ "||"] = Operator( "||", 15 ) ;

    Operators[ "?"]  = Operator( "?",  16 ) ;
    Operators[ ":"]  = Operator( ":",  16 ) ;

    Operators[ "="]  = Operator( "=",  18, 2, true, "01" ) ;
    Operators[ "+="] = Operator( "+=", 18, 2, true, "01" ) ;
    Operators[ "-="] = Operator( "-=", 18, 2, true, "01" ) ;
    Operators[ "*="] = Operator( "*=", 18, 2, true, "01" ) ;
    Operators[ "/="] = Operator( "/=", 18, 2, true, "01" ) ;
    Operators[ "%="] = Operator( "%=", 18, 2, true, "01" ) ;

    Operators[ ","]  = Operator( ",",  19, 2, true, "01" ) ;

    Operators[ ";"]  = Operator( ";",  100 ) ;

    Keywords[ "true"]    = Data( "true",   CONSTANT ) ;
    Keywords[ "false"]   = Data( "false",  CONSTANT ) ;
  
    Keywords[ "bool"]    = Data( "bool",   DATATYPE ) ;
    Keywords[ "char"]    = Data( "char",   DATATYPE ) ;
    Keywords[ "string"]  = Data( "string", DATATYPE ) ;
    Keywords[ "int"]     = Data( "int",    DATATYPE ) ;
    Keywords[ "float"]   = Data( "float",  DATATYPE ) ;
    Keywords[ "void"]    = Data( "void",   DATATYPE ) ;

    Keywords[ "else"]    = Data( "else",   ELSE ) ;

    Keywords[ "return"]  = Data( "return", RETURN ) ;
    
    Keywords[ "while"]   = Data( "while",  CONDITIONAL_CONTROLFLOW ) ;
    Keywords[ "if"]      = Data( "if",     CONDITIONAL_CONTROLFLOW ) ;

    Functions[ "Done"]             = Function( "Done",             VOID ) ;
    Functions[ "ListAllVariables"] = Function( "ListAllVariables", VOID ) ;
    Functions[ "ListVariable"]     = Function( "ListVariable",     VOID ) ;
    Functions[ "ListAllFunctions"] = Function( "ListAllFunctions", VOID ) ;
    Functions[ "ListFunction"]     = Function( "ListFunction",     VOID ) ;

    subroutines[0].variables[ "cin"]  = Data( "cin",  OBJECT ) ;
    subroutines[0].variables[ "cout"] = Data( "cout", OBJECT ) ;
  } // TableSetup()


  void Reset() {
    subroutines.resize( 1 ) ;
    TableSetup() ;
  } // Reset()


  bool Execute() {
    return Execute( subroutines.back() .operations.back() ) ;
  } // Execute()


  bool Execute( Operation & operation ) {
    operation.DebugPrint() ;

    string header = ! operation.exp.empty() ? operation.exp[0].GetToken() .str : "";
    cout << "> ";

    if ( operation.mode == ERROR_OPERATION ) 
      cout << operation.exp.back() .GetToken() .str << "\n";
    
    else if ( operation.mode == OPERATION ) 
      return RunOperation( operation.exp ) ;
    
    else if ( operation.mode == CONTROLFLOW_OPERATION ) 
      RunControlflow( operation.exp ) ;
    
    else if ( operation.mode == VARIABLE_DECLARATION ) 
      VariableDeclaration( operation.exp, true ) ;

    else if ( operation.mode == FUNCTION_DECLARATION ) 
      FunctionDeclaration( operation.exp ) ;

    return true;
  } // Execute()


  bool RunOperation( vector < Data > & exp ) {
    if ( ! exp.empty() ) {
      vector < Data > operands;
      Data data;
      Token token;

      token = exp[0].GetToken() ;
      if ( exp[0].type == FUNCTION ) {
        if ( token.str == "Done" ) 
          return false;

        else if ( token.str == "ListAllVariables" ) 
          ListAllVariables() ;

        else if ( token.str == "ListVariable" && exp.size() == 2 ) 
          ListVariable( exp[1].GetToken() .str ) ;

        else if ( token.str == "ListAllFunctions" ) 
          ListAllFunctions() ;

        else if ( token.str == "ListFunction" && exp.size() == 2 ) 
          ListFunction( exp[1].GetToken() .str ) ;
      } // if

    } // if

    
    cout << "Statement executed ...\n";
    return true;
  } // RunOperation()


  void RunControlflow( vector < Data > & exp ) {
    string header = exp[0].GetToken() .str;
    cout << "Statement executed ...\n";
  } // RunControlflow()


  void VariableDeclaration( vector < Data > & exp, bool printout ) {
    DataType dataType = string_to_dataType( exp[0].GetToken() .str ) ;

    for ( int k = 1 ; k < exp.size() ; k ++ ) {
      string var = exp[k].data[0].str;
      int amount = exp[k].data.size() > 1 ? exp[k].data[1].inum : 0;

      if ( printout && IsMain() ) {
        if ( FindVariable( var ) >= 0 ) 
          cout << "New definition of " << var << " entered ...\n";
        else cout << "Definition of " << var << " entered ...\n";

      } // if

      // debug("(var-dec:"+exp[0].GetToken().str+"'"+var+"'");
      // if(amount)
      //    debug("["+to_string(amount)+"]");
      // debug(")");

      if ( amount == 0 ) 
        subroutines.back() .variables[var] = Data( "", dataType ) ;
      else {
        subroutines.back() .variables[var] = Data( ARRAY ) ;
        Token temp = Token( "", dataType ) ;
        subroutines.back() .variables[var].data.resize( amount, temp ) ;
      } // else

    } // for

  } // VariableDeclaration()


  void FunctionDeclaration( vector < Data > & exp ) {
    // define function
    
    DataType funcDataType = string_to_dataType( exp[0].GetToken() .str ) ;
    string funcName = exp[1].GetToken() .str;
    vector < Parameter > params;
    Parameter param;

    vector < Data > var_dec_exp;
    string var;
    string routineName;
    bool is_reference;
    string showup;

    if ( IsFunction( funcName ) ) 
      cout << "New definition of " << funcName << "() entered ...\n";
    else
      cout << "Definition of " << funcName << "() entered ...\n";

    for ( int k = 2 ; k < exp.size() - 2 ; k ++ ) {
      if ( exp[ ++ k].GetToken() .str == "&" ) {
        is_reference = true;
        ++ k;
      } // if

      param = Parameter( exp[k].data[0].str, is_reference ) ;
      params.push_back( param ) ;
    } // for


    routineName = exp[exp.size() - 2].GetToken() .str;
    showup = exp[exp.size() - 1].GetToken() .str;

    Functions[funcName] = Function( funcName, funcDataType, params, routineName, showup ) ;
  } // FunctionDeclaration()


  bool IsOperator( const string & str ) {
    return Operators.find( str ) != Operators.end() ;
  } // IsOperator()


  bool IsKeyword( const string & str ) {
    return Keywords.find( str ) != Keywords.end() ;
  } // IsKeyword()


  bool IsFunction( const string & str ) {
    return Functions.find( str ) != Functions.end() ;
  } // IsFunction()


  bool IsMain() {
    return subroutines.size() == 1;
  } // IsMain()


  Token ResolveData( Data & data ) {
    Token token = data.GetToken() ;

    if ( token.type == SYMBOL ) {
      int k = FindVariable( token.str ) ;
      if ( k >= 0 ) {
        if ( data.data.size() > 1 ) {
          int arr_index = data.data[0].inum;
          return subroutines[k].variables[token.str].data[arr_index];
        } // if

        else
          return subroutines[k].variables[token.str].GetToken() ;
      } // if

      else
        return Token( "undefined identifier : '" + token.str + "'", ERROR ) ;
    } // if


    return token;
  } // ResolveData()


  int FindVariable( string & var ) {
    for ( int k = subroutines.size() - 1 ; k >= 0 ; k -- ) 
      if ( subroutines[k].variables.find( var ) != subroutines[k].variables.end() ) 
        return k;

    return - 1;
  } // FindVariable()


  Data Operate( Operator & op, vector < Data > & exp ) {
    if ( op.operands == 1 ) {
      Data da = exp.back() ;
      exp.pop_back() ;
      Data result = Operate( op, da ) ;
      exp.push_back( result ) ;
      return result;
    } // if


    if ( op.operands == 2 ) {
      Data db = exp.back() ;
      exp.pop_back() ;
      Data da = exp.back() ;
      exp.pop_back() ;
      Data result = Operate( op, da, db ) ;
      exp.push_back( result ) ;
      return result;
    } // if


    if ( op.operands == 3 ) {
      Data dc = exp.back() ;
      exp.pop_back() ;
      Data db = exp.back() ;
      exp.pop_back() ;
      Data da = exp.back() ;
      exp.pop_back() ;
      Data result = Operate( op, da, db, dc ) ;
      exp.push_back( result ) ;
      return result;
    } // if


    return Data() ;
  } // Operate()


  Data Operate( Operator & op, Data & da ) {
    Token a = ( op.resovable[0] == '1' ) ? ResolveData( da ) : da.GetToken() ;
    if ( a.type == ERROR ) return Data( a ) ;

    string var = da.GetToken() .str;
    int k = FindVariable( var ) ;

    Token result = op.str == "++" ? 
              op.rightass ? PLUS_before_return( subroutines[k].variables[var] ) : 
                     PLUS_after_return( subroutines[k].variables[var], a ) : 
              op.str == "--" ? 
              op.rightass ? MINUS_before_return( subroutines[k].variables[var] ) : 
                     MINUS_after_return( subroutines[k].variables[var], a ) : 
              op.str == "!"  ? NOT( a ) : 
              op.str == "+"  ? POSITIVE( a ) : 
              op.str == "-"  ? NEGATIVE( a ) : Token() ;

    return Data( result ) ;
  } // Operate()


  Data Operate( Operator & op, Data & da, Data & db ) {
    Token a = ( op.resovable[0] == '1' ) ? ResolveData( da ) : da.GetToken() ;
    Token b = ( op.resovable[1] == '1' ) ? ResolveData( db ) : db.GetToken() ;
    if ( a.type == ERROR ) return Data( a ) ;
    if ( b.type == ERROR ) return Data( b ) ;

    Data result = op.str == "[]" ? INDEX( da, b ) : 
           op.str == "*"  ? MUL( a, b )     : 
           op.str == "/"  ? DIV( a, b )     : 
           op.str == "+"  ? ADD( a, b )     : 
           op.str == "-"  ? SUB( a, b )     : 
           op.str == ">>" ? SR( a, b )      : 
           op.str == "<<" ? SL( a, b )      : 
           op.str == ">"  ? GT( a, b )      : 
           op.str == "<"  ? LT( a, b )      : 
           op.str == ">=" ? GE( a, b )      : 
           op.str == "<=" ? LE( a, b )      : 
           op.str == "==" ? EQ( a, b )      : 
           op.str == "!=" ? NEQ( a, b )     : 
           op.str == "&&" ? AND( a, b )     : 
           op.str == "||" ? OR( a, b )      : 
           op.str == "="  ? ASSIGN( da, b ) : 
           op.str == "+=" ? ASSIGN( da, b ) : 
           op.str == "-=" ? ASSIGN( da, b ) : 
           op.str == "*=" ? ASSIGN( da, b ) : 
           op.str == "/=" ? ASSIGN( da, b ) : 
           op.str == "%=" ? ASSIGN( da, b ) : 
           op.str == ","  ? COMMA( da, b )  : Data() ;

    return result;
  } // Operate()


  Data Operate( Operator & op, Data & da, Data & db, Data & dc ) {
    Token a = ( op.resovable[0] == '1' ) ? ResolveData( da ) : da.GetToken() ;
    Token b = ( op.resovable[1] == '1' ) ? ResolveData( db ) : db.GetToken() ;
    Token c = ( op.resovable[2] == '1' ) ? ResolveData( dc ) : dc.GetToken() ;
    if ( a.type == ERROR ) return Data( a ) ;
    if ( b.type == ERROR ) return Data( b ) ;
    if ( c.type == ERROR ) return Data( c ) ;

    Data result = op.str == "?:" ? TERNARY( a, b, c ) : Data() ;

    return result;
  } // Operate()


  Data OperateFunction( vector < Data > & exp ) {
    Data params = exp.back() ;
    exp.pop_back() ;
    string funcName = exp.back() .GetToken() .str;
    exp.pop_back() ;

    // 

    DataType dataType = Functions[funcName].dataType;
    Data result = Data( "", dataType ) ;
    exp.push_back( result ) ;
    return result;
  } // OperateFunction()


  // Unary Operations
  Token PLUS_before_return( Data & var ) {
    var.data.back() .inum += 1;
    var.data.back() .fnum += 1;
    return var.GetToken() ;
  } // PLUS_before_return()


  Token PLUS_after_return( Data & var, Token & prev ) {
    var.data.back() .inum += 1;
    var.data.back() .fnum += 1;
    return prev;
  } // PLUS_after_return()

  
  Token MINUS_before_return( Data & var ) {
    var.data.back() .inum -= 1;
    var.data.back() .fnum -= 1;
    return var.GetToken() ;
  } // MINUS_before_return()

  
  Token MINUS_after_return( Data & var, Token & prev ) {
    var.data.back() .inum -= 1;
    var.data.back() .fnum -= 1;
    return prev;
  } // MINUS_after_return()


  Token POSITIVE( Token & a ) {
    return a;
  } // POSITIVE()


  Token NEGATIVE( Token & a ) {
    a.inum = - a.inum;
    a.fnum = - a.fnum;
    return a;
  } // NEGATIVE()


  Token NOT( Token & a ) {
    return Token( ! a.inum ) ;
  } // NOT()


  // Binary Operations
  Data INDEX( Data & a, Token & b ) {
    a.data.insert( a.data.begin(), b ) ;
    return a;
  } // INDEX()


  Data MUL( Token & a, Token & b ) {
    Token temp;

    if ( a.type == INT && b.type == INT ) 
      temp = Token( a.inum * b.inum ) ;
    else
      temp = Token( a.fnum * b.fnum ) ;

    return Data( temp ) ;
  } // MUL()


  Data DIV( Token & a, Token & b ) {
    Token temp;

    if ( a.type == INT && b.type == INT ) 
      temp = Token( a.inum / b.inum ) ;
    else
      temp = Token( a.fnum / b.fnum ) ;

    return Data( temp ) ;
  } // DIV()


  Data ADD( Token & a, Token & b ) {
    Token temp;

    if ( a.type == INT && b.type == INT ) 
      temp = Token( a.inum + b.inum ) ;
    else
      temp = Token( a.fnum + b.fnum ) ;

    return Data( temp ) ;
  } // ADD()


  Data SUB( Token & a, Token & b ) {
    Token temp;

    if ( a.type == INT && b.type == INT ) 
      temp = Token( a.inum - b.inum ) ;
    else
      temp = Token( a.fnum - b.fnum ) ;

    return Data( temp ) ;
  } // SUB()


  Data SR( Token & a, Token & b ) {
    if ( a.type == OBJECT ) 
      return Data( a ) ;
    
    Token temp = Token( a.inum >> b.inum ) ;
    return Data( temp ) ;
  } // SR()


  Data SL( Token & a, Token & b ) {
    if ( a.type == OBJECT ) 
      return Data( a ) ;
    
    Token temp = Token( a.inum << b.inum ) ;
    return Data( temp ) ;
  } // SL()


  Data GT( Token & a, Token & b ) {
    Token temp = Token( a.type == INT && b.type == INT ? ( a.inum > b.inum ) : ( a.fnum > b.fnum ) ) ;
    return Data( temp ) ;
  } // GT()


  Data LT( Token & a, Token & b ) {
    Token temp = Token( a.type == INT && b.type == INT ? ( a.inum < b.inum ) : ( a.fnum < b.fnum ) ) ;
    return Data( temp ) ;
  } // LT()


  Data GE( Token & a, Token & b ) {
    Token temp = Token( a.type == INT && b.type == INT ? ( a.inum >= b.inum ) : ( a.fnum >= b.fnum ) ) ;
    return Data( temp ) ;
  } // GE()


  Data LE( Token & a, Token & b ) {
    Token temp = Token( a.type == INT && b.type == INT ? ( a.inum <= b.inum ) : ( a.fnum <= b.fnum ) ) ;
    return Data( temp ) ;
  } // LE()


  Data EQ( Token & a, Token & b ) {
    Token temp = Token( a.type == INT && b.type == INT ? ( a.inum == b.inum ) : ( a.fnum == b.fnum ) ) ;
    return Data( temp ) ;
  } // EQ()


  Data NEQ( Token & a, Token & b ) {
    Token temp = Token( a.type == INT && b.type == INT ? ( a.inum != b.inum ) : ( a.fnum != b.fnum ) ) ;
    return Data( temp ) ;
  } // NEQ()


  Data AND( Token & a, Token & b ) {
    Token temp = Token( a.inum && b.inum ) ;
    return Data( temp ) ;
  } // AND()


  Data OR( Token & a, Token & b ) {
    Token temp = Token( a.inum || b.inum ) ;
    return Data( temp ) ;
  } // OR()


  // Binary Assignment Operations
  Data ASSIGN( Data & a, Token & b ) {
    debug( "(assign)" ) ;
    string var = a.GetToken() .str;
    int k = FindVariable( var ) ;

    if ( a.data.size() > 1 ) {
      int arr_index = a.data[0].inum;

      subroutines[k].variables[var].data[arr_index].str = b.str;
      subroutines[k].variables[var].data[arr_index].inum = b.inum;
      subroutines[k].variables[var].data[arr_index].fnum = b.fnum;

      return subroutines[k].variables[var].data[arr_index];
    } // if

    else {
      subroutines[k].variables[var].data.back() .str = b.str;
      subroutines[k].variables[var].data.back() .inum = b.inum;
      subroutines[k].variables[var].data.back() .fnum = b.fnum;

      return subroutines[k].variables[var].data.back() ;
    } // else

  } // ASSIGN()


  Data COMMA( Data & a, Token & b ) {
    a.data.push_back( b ) ;
    return a;
  } // COMMA()


  Data TERNARY( Token & a, Token & b, Token & c ) {
    if ( a.inum != 0 ) 
      return Data( b ) ;

    return Data( c ) ;
  } // TERNARY()


  void ListAllVariables() {
    map < string, Data > :: iterator it;
    for ( it = subroutines[0].variables.begin() ; it != subroutines[0].variables.end() ; it ++ ) 
      if ( it -> second.type != OBJECT ) 
        cout << it -> first << '\n';
  } // ListAllVariables()


  void ListVariable( string str ) {
    string varname;
    for ( int chindex = 1 ; chindex < str.size() - 1 ; chindex ++ ) 
      varname += str[chindex];

    int k = FindVariable( varname ) ;
    Data data = subroutines[k].variables[varname];
    DataType type = data.data[0].type;

    string result = type == BOOLEAN ? "bool" : 
            type == INT     ? "int" : 
            type == FLOAT   ? "float" : 
            type == STRING  ? "string" : 
            type == CHAR    ? "char"  : "";

    result += " " + varname;

    if ( data.type == ARRAY ) {
      int amount = data.data.size() ;
      result += "[ " + to_string( amount ) + " ]";
    } // if


    cout << result << " ;\n";
  } // ListVariable()


  void ListAllFunctions() {
    map < string, Function > :: iterator it;
    for ( it = Functions.begin() ; it != Functions.end() ; it ++ ) 
      if ( it -> second.showlist == true ) 
        cout << it -> first << "()\n";
  } // ListAllFunctions()


  void ListFunction( string str ) {
    string funcname;
    for ( int chindex = 1 ; chindex < str.size() - 1 ; chindex ++ ) 
      funcname += str[chindex];

    cout << Functions[funcname].showup;
  } // ListFunction()

} ;

class Parser {
public : 
  char ch;
  string code;
  int index, len;
  int line;
  bool pushed;
  bool line_reset;
  bool delayed;
  bool controlflow_delayed;
  bool token_delayed;

  Token token;
  Token prevToken;
  Data  data;
  Data  prevData;
  Operation operation;

  vector < Data > opstack;
  vector < Data > subject;
  vector < string > parentheses;
  int routineID;

  vector < Token > tokens;
  vector < int > tokens_layer;

  void Setup( string & inputCode ) {
    code.swap( inputCode ) ;
    len = code.size() ;
    routineID = 1; // 0 for main
    index = 0;
    line = 1;
    pushed = false;
    line_reset = false;
    token_delayed = false;
    delayed = false;
    opstack.clear() ;
    subject.clear() ;
    parentheses.clear() ;
  } // Setup()


  bool Parse( Program & program ) {
    if ( token_delayed ) {
      token_delayed = false;
      PushToken( program ) ;
    } // if


    while ( index < len ) {
      if ( index && code[index - 1] == '\n' ) {
        if ( line_reset ) {
          // debug("(reline)");
          line = 1;
          line_reset = false;
        } // if

        else {
          line ++ ;
          // debug("(line:"+to_string(line)+")");
        } // else

      } // if


      ch = code[index ++ ];
      string tempstr;
      tempstr += ch;
      debugY( tempstr ) ;

      if ( token.str == "" ) {
        if ( ! is_whitespace( ch ) ) {
          if ( ch == '\'' ) 
            GetChar() ;

          else if ( ch == '"' ) 
            GetString() ;

          else {
            token.str = ch;
            token.type = program.IsOperator( token.str )  ? OPERATOR         : 
              ch == '(' || ch == '[' || ch == '{' ? LEFT_PARENTHESE  : 
              ch == ')' || ch == ']' || ch == '}' ? RIGHT_PARENTHESE : 
              char_is_number( ch )                ? INT              : 
              char_is_symbol( ch )                ? SYMBOL           : 
              ch == '.'                         ? FLOAT            : NONE;
          } // else

        } // if

      } // if

      else {
        if ( program.IsOperator( token.str + ch ) ) {
          token.str.push_back( ch ) ;
          token.type = OPERATOR;
        } // if


        else if ( token.type == SYMBOL && char_is_symbol( ch ) ) 
          token.str.push_back( ch ) ;

        else if ( ( token.type == INT || token.type == FLOAT ) && char_is_number( ch ) ) 
          token.str.push_back( ch ) ;

        else if ( token.type == INT && ch == '.' ) {
          token.str.push_back( ch ) ;
          token.type = FLOAT;
        } // else if


        else if ( ch == '/' && token.str == "/" ) 
          Commented() ;
        
        else {
          if ( ! is_whitespace( ch ) ) 
            -- index;

          PushToken( program ) ;
        } // else

      } // else


      if ( pushed ) {
        pushed = false;
        return true;
      } // if

    } // while


    if ( token.str != "" ) {
      PushToken( program ) ;
      if ( pushed ) 
        return true;
    } // if


    return false;
  } // Parse()


  void PushToken( Program & program ) {

    // debug("(push-token:"+token.GetString()+")");
    // debug("(prev-data="+prevData.GetToken().GetString()+")");

    token = Token( token.str, token.type ) ;

    Token temp = token;

    while ( delayed ) {
      delayed = false;
      if ( token.str != "else if" && token.str != "else" ) {
        if ( parentheses.empty() || parentheses.back() != "{" ) {
          if ( program.IsMain() ) {
            debug( "(token-delayed)" ) ;
            token_delayed = true;
            ResetOperation( program ) ;
            token = temp;
            return;
          } // if

          else
            EndRoutine( program ) ;
        } // if

      } // if

    } // while

    token = temp;

    if ( controlflow_delayed ) {
      controlflow_delayed = false;

      if ( prevData.GetToken() .str == "else" ) {
        if ( token.str == "if" ) {
          debug( "(else-if)" ) ;
          prevData = operation.exp.back() = subject.back() = Data( "else if", CONDITIONAL_CONTROLFLOW ) ;
          tokens.back() = Token( "else if", CONDITIONAL_CONTROLFLOW ) ;
          token.Reset() ;
          // controlflow_delayed = true;
          return;
        } // if

      } // if


      if ( token.str == "{" ) {
        Token tempt = Token( "{", NONE ) ;
        tokens.push_back( tempt ) ;
        tokens_layer.push_back( 0 ) ;
        AppendSubroutine( program, "{" ) ;
        return;
      } // if

      else
        AppendSubroutine( program, "" ) ;

      token = temp;
    } // if

    

    if ( prevData.GetToken() .str == "else" ) {
      if ( token.str != "if" ) {
        debug( "(here)" ) ;
        AppendSubroutine( program, "" ) ;
        token = temp;
      } // if


      else {
        debug( "(else-if)" ) ;
        prevData = operation.exp.back() = subject.back() = Data( "else if", CONDITIONAL_CONTROLFLOW ) ;
        tokens.back() = Token( "else if", CONDITIONAL_CONTROLFLOW ) ;
        token.Reset() ;
        return;
      } // else

    } // if


    if ( token.type == SYMBOL && program.IsKeyword( token.str ) ) {
      token.type = program.Keywords[token.str].type;

      if ( token.type == CONSTANT ) {
        token = token.str == "true"  ? Token( "true",  BOOLEAN ) : 
            token.str == "false" ? Token( "false", BOOLEAN ) : token;
      } // if

    } // if

    
    // adjust operator
    if ( token.type == OPERATOR ) {
      token.op = program.Operators[token.str];

      if ( ( token.str == "+" || token.str == "-" ) && ! prevData.IsOperand() ) 
        token.op = Operator( token.str, 3, 1, true, "1" ) ;

      if ( ( token.str == "++" || token.str == "--" ) && ! prevData.IsOperand() ) 
        token.op = Operator( token.str, 3, 1, true, "1" ) ;
    } // if

      
    data = Data( token ) ;
    prevToken = prevData.GetToken() ;
    int layer = program.subroutines.size() ;
    tokens.push_back( token ) ;
    tokens_layer.push_back( layer ) ;

    if ( ! LexicalAnalyze() ) {
      debug( "(lexical error:'" + token.str + "')" ) ;
      string error_msg = "unrecognized token with first char : '" + token.str.substr( 0, 1 ) + "'";
      SetError( error_msg, program ) ;
      return;
    } // if

    // debug("(lexical-done)");

    if ( ! SyntaxAnalyze( program ) ) {
      debug( "(syntax error)" ) ;
      string error_msg = "unexpected token : '" + token.str + "'";
      SetError( error_msg, program ) ;
      return;
    } // if

    // debug("(syntax-done)");

    if ( ! SemanticAnalyze( program ) ) {
      debug( "(semantic error)" ) ;
      string error_msg = prevData.GetToken() .str;
      SetError( error_msg, program ) ;
      return;
    } // if

    // debug("(semantic-done)");

    token.Reset() ;
  } // PushToken()


  bool LexicalAnalyze() {
    // Unrecognized Error
    return ! ( token.type == NONE || ( token.type == FLOAT && ! is_float( token.str ) ) ) ;
  } // LexicalAnalyze()


  bool SyntaxAnalyze( Program & program ) {

    // declaring operation format check
    if ( operation.mode == DECLARATION ) {
      if ( prevToken.type == DATATYPE ) {
        if ( token.type != SYMBOL ) {
          return false;
        } // if

      } // if


      else if ( prevToken.type == SYMBOL ) {
        if ( token.str != "(" && token.str != "," && token.str != ";" && token.str != "[" ) 
          return false;

        if ( token.str == "(" && program.subroutines.size() > 1 ) 
          return false;
      } // else if

    } // if


    else if ( operation.mode == VARIABLE_DECLARATION ) {
      if ( prevToken.str == "," ) {
        if ( token.type != SYMBOL ) 
          return false;
      } // if


      else if ( prevToken.type == SYMBOL ) {
        if ( token.str != "," && token.str != ";" && token.str != "[" ) 
          return false;
      } // else if


      else if ( prevToken.str == "[" ) {
        if ( token.type != INT ) 
          return false;
      } // else if


      else if ( prevToken.type == INT ) {
        if ( token.str != "]" ) 
          return false;
      } // else if


      else if ( prevToken.str == "]" ) {
        if ( token.str != "," && token.str != ";" ) 
          return false;
      } // else if

    } // else if


    else if ( operation.mode == FUNCTION_DECLARATION ) {
      if ( prevToken.str == "void" ) {
        if ( token.str != ")" ) 
          return false;
      } // if


      else if ( prevToken.str == "(" ) {
        if ( token.type != DATATYPE && token.str != ")" ) 
          return false;
      } // else if


      else if ( prevToken.type == DATATYPE ) {
        if ( token.type != SYMBOL && token.str != "&" ) 
          return false;
      } // else if


      else if ( prevToken.str == "&" ) {
        if ( token.type != SYMBOL ) 
          return false;
      } // else if


      else if ( prevToken.type == SYMBOL ) {
        if ( token.str != "," && token.str != "[" && token.str != ")" ) 
          return false;
      } // else if


      else if ( prevToken.str == "[" ) {
        if ( token.type != INT ) 
          return false;
      } // else if


      else if ( prevToken.type == INT ) {
        if ( token.str != "]" ) 
          return false;
      } // else if


      else if ( prevToken.str == "]" ) {
        if ( token.str != "," && token.str != ")" ) 
          return false;
      } // else if


      else if ( prevToken.str == "," ) {
        if ( token.type != DATATYPE ) 
          return false;
      } // else if


      else if ( prevToken.str == ")" ) {
        if ( token.str != "{" ) 
          return false;
      } // else if

    } // else if


    else {
      // backward check
      if ( prevData.type == OPERATOR ) {
        if ( prevToken.op.operands == 1 && prevToken.op.rightass ) {
          if ( ! token.IsOperand() && token.str != "(" ) 
            return false;
        } // if


        if ( prevToken.op.operands > 1 ) {
          if ( ! token.IsOperand() && token.str != "(" && token.op.operands != 1 ) 
            return false;
        } // if


        if ( ( prevToken.str == "++" || prevToken.str == "--" ) && prevToken.op.rightass ) {
          if ( token.type != SYMBOL ) 
            return false;
        } // if

      } // if


      else if ( prevData.type == CONDITIONAL_CONTROLFLOW ) {
        if ( token.str != "(" ) 
          return false;
      } // else if


      // forward check
      if ( token.type == OPERATOR ) {
        // cout << "(op-prevdata="<<prevData.GetToken().inum<<"("<<prevData.GetToken().type<<")"<<")";
        if ( token.str != ";" ) {
          if ( ! token.op.rightass && ! prevData.IsOperand() ) 
            return false;

          if ( ( token.str == "++" || token.str == "--" ) && ! token.op.rightass ) {
            if ( prevToken.type != SYMBOL ) 
              return false;
          } // if


          bool cond = token.str == "="  || token.str == "+=" || token.str == "-=" || 
               token.str == "*=" || token.str == "/=" || token.str == "%=";
          if ( cond ) {
            if ( prevData.type != SYMBOL ) 
              return false;

            if ( ! opstack.empty() ) {
              int prior = opstack.back() .GetToken() .op.priority;                        
              if ( 0 < prior && prior < 17 ) 
                return false;
            } // if

          } // if

        } // if

      } // if


      else if ( token.IsOperand() ) {
        if ( prevData.type == OPERATOR ) {
          if ( prevToken.op.operands == 1 && ! prevToken.op.rightass ) 
            return false;
        } // if

        else if ( ! ( prevData.type == NONE || ( prevData.type == TUPLE && prevData.data.empty() ) ) ) 
          return false;
      } // else if


      else if ( token.type == ELSE ) {
        if ( ! subject.empty() ) 
          return false;
          
        string prevOp = "";
        bool cond = program.subroutines.back() .operations.size() > 0 && 
              program.subroutines.back() .operations.back() .exp.size() > 0;

        if ( cond ) 
          prevOp = program.subroutines.back() .operations.back() .exp[0].GetToken() .str;

        if ( token.str == "else" && prevOp != "if" && prevOp != "else if" ) 
          return false;
      } // else if


      else if ( token.type == RETURN ) {
        if ( ! subject.empty() ) 
          return false;
      } // else if


      else if ( token.type == CONDITIONAL_CONTROLFLOW ) {
        if ( ! subject.empty() ) 
          return false;

        string prevOp = "";
        bool cond = program.subroutines.back() .operations.size() > 0 && 
              program.subroutines.back() .operations.back() .exp.size() > 0;
              
        if ( cond ) 
          prevOp = program.subroutines.back() .operations.back() .exp[0].GetToken() .str;

        if ( token.str == "else if" && prevOp != "else if" && prevOp != "if" ) 
          return false;
      } // else if


      else if ( token.type == DATATYPE ) {
        if ( ! subject.empty() ) 
          return false;
      } // else if


      else if ( token.str == "(" ) {
        // debug("(leftp-prev-data:"+prevData.GetString()+")");

        if ( ! ( prevData.type == OPERATOR || prevData.type == FUNCTION || 
                 prevData.type == CONDITIONAL_CONTROLFLOW || prevData.type == NONE ) ) 
          return false;
      } // else if


      else if ( token.str == "[" ) {
        if ( prevData.type != SYMBOL ) 
          return false;
      } // else if


      else if ( token.str == "{" ) {
        if ( ! operation.exp.empty() ) 
          return false;
      } // else if


      else if ( token.type == RIGHT_PARENTHESE ) {
        if ( parentheses.empty() ) 
          return false;

        if ( token.str == ")" && parentheses.back() != "(" ) 
          return false;

        if ( token.str == "]" && parentheses.back() != "[" ) 
          return false;

        if ( token.str == "}" && parentheses.back() != "{" ) 
          return false;

        if ( token.str == "}" && ! operation.exp.empty() ) 
          return false;

        if ( token.str == ")" && ! ( ( prevData.IsOperand() || prevData.type == TUPLE ) ) ) 
          return false;
      } // else if


    } // else


    return true;
  } // SyntaxAnalyze()


  bool SemanticAnalyze( Program & program ) {

    // declaring operation
    if ( operation.mode == DECLARATION ) {
      if ( token.str == "(" ) 
        operation.mode = FUNCTION_DECLARATION;

      else if ( token.str == "," || token.str == ";" || token.str == "[" ) 
        operation.mode = VARIABLE_DECLARATION;

      else
        operation.exp.push_back( data ) ;

      prevData = data;
    } // if


    else if ( operation.mode == FUNCTION_DECLARATION ) {
      if ( token.str == "{" ) 
        AppendSubroutine( program, "{" ) ;

      else if ( token.str != "," && token.str != ")" && token.str != "[" && token.str != "]" ) {
        if ( token.type == INT ) 
          operation.exp.back() .data.push_back( token ) ;
        else
          operation.exp.push_back( data ) ;
      } // else if


      prevData = data;
    } // else if


    else if ( operation.mode == VARIABLE_DECLARATION ) {
      if ( token.type != OPERATOR && token.str != "[" && token.str != "]" ) {
        if ( token.type == INT ) 
          operation.exp.back() .data.push_back( token ) ;
        else
          operation.exp.push_back( data ) ;
      } // if


      prevData = data;
    } // else if


    else {
      if ( token.type == OPERATOR ) {
        if ( token.op.operands == 1 ) {
          if ( token.op.rightass ) {
            opstack.push_back( data ) ;
            prevData = data;
          } // if

          else {
            operation.exp.push_back( data ) ;
            
            // evaluate unary operation
            prevData = program.Operate( token.op, subject ) ;
            if ( prevData.type == ERROR ) 
              return false;
          } // else

        } // if

        else {
          bool breakout;
          bool cond = ! opstack.empty() && 
             token.op.priority >= opstack.back() .GetToken() .op.priority && 
             opstack.back() .type != LEFT_PARENTHESE;

          while ( cond ) {
            breakout = false;
            Operator op = opstack.back() .GetToken() .op;
            
            if ( op.str == "?" ) {
              if ( token.str == ":" ) {
                op = Operator( "?:", 16, 3, true, "111" ) ;
                Token tempt = Token( op ) ;
                opstack.back() = Data( tempt ) ;
                prevData = opstack.back() ;
                return true;
              } // if

              else if ( token.str == "?" ) {
                breakout = true;
              } // else if

              else {
                prevData = Data( "unexpected token : '" + token.str + "'", ERROR ) ;
                return false;
              } // else

            } // if

            else {
              operation.exp.push_back( opstack.back() ) ;
              opstack.pop_back() ;

              // evaluate binary operation
              prevData = program.Operate( op, subject ) ;
              if ( prevData.type == ERROR ) 
                return false;
            } // else


            cond = ! breakout && ! opstack.empty() && 
             token.op.priority >= opstack.back() .GetToken() .op.priority && 
             opstack.back() .type != LEFT_PARENTHESE;
          } // while


          if ( token.str == ":" ) {
            prevData = Data( "unexpected token : '" + token.str + "'", ERROR ) ;
            return false;
          } // if


          opstack.push_back( data ) ;
          prevData = data;
        } // else


        debug( "\n(opstack:{ " ) ;
        for ( int x = 0 ; x < opstack.size() ; x ++ ) 
          debug( "'" + opstack[x].GetToken() .str + "' " ) ;
        debug( "})" ) ;
        debug( "(subject:{ " ) ;
        for ( int x = 0 ; x < subject.size() ; x ++ ) 
          debug( subject[x].GetString() + " " ) ;
        debug( "})\n" ) ;
      } // if


      else if ( token.IsOperand() ) {
        if ( program.IsFunction( token.str ) ) {
          debug( "(is-function)" ) ;
          token.type = FUNCTION;
          data = Data( token ) ;
        } // if


        if ( prevData.type == TUPLE && prevData.data.empty() ) {
          operation.exp.back() .data.push_back( token ) ;
          subject.back() .data.push_back( token ) ;
          prevData = subject.back() ;
        } // if

        else {
          operation.exp.push_back( data ) ;
          subject.push_back( data ) ;
          prevData = data;
          debug( "(" + subject.back() .GetString() + ")" ) ;

          if ( token.type == SYMBOL ) {
            debug( "(this)" ) ;
            Token resolved = program.ResolveData( data ) ;
            if ( resolved.type == ERROR ) {
              prevData = Data( resolved ) ;
              return false;
            } // if

          } // if

        } // else

      } // else if


      else if ( token.type == DATATYPE ) {
        operation.exp.push_back( data ) ;
        subject.push_back( data ) ;
        prevData = data;
        operation.mode = DECLARATION;
      } // else if


      else if ( token.type == ELSE ) {
        operation.exp.push_back( data ) ;
        subject.push_back( data ) ;
        prevData = data;
        operation.mode = CONTROLFLOW_OPERATION;
        controlflow_delayed = true;
      } // else if


      else if ( token.type == RETURN ) {
        operation.exp.push_back( data ) ;
        subject.push_back( data ) ;
        prevData.Reset() ;
      } // else if


      else if ( token.type == CONDITIONAL_CONTROLFLOW ) {
        operation.exp.push_back( data ) ;
        subject.push_back( data ) ;
        prevData = data;
        operation.mode = CONTROLFLOW_OPERATION;
      } // else if


      else if ( token.type == LEFT_PARENTHESE ) {
        if ( token.str == "(" ) {
          parentheses.push_back( token.str ) ;
          opstack.push_back( data ) ;

          if ( prevData.type == FUNCTION ) {
            prevData = Data( TUPLE ) ;
            operation.exp.push_back( prevData ) ;
            subject.push_back( prevData ) ;
          } // if

          else        
            prevData.Reset() ;
        } // if


        else if ( token.str == "[" ) {
          parentheses.push_back( token.str ) ;
          opstack.push_back( data ) ;
          prevData.Reset() ;
        } // else if


        else if ( token.str == "{" ) 
          AppendSubroutine( program, "{" ) ;
      } // else if


      else if ( token.type == RIGHT_PARENTHESE ) {
        if ( token.str == ")" || token.str == "]" ) {
          parentheses.pop_back() ;

          while ( ! opstack.empty() && opstack.back() .GetToken() .type != LEFT_PARENTHESE ) {
            operation.exp.push_back( opstack.back() ) ;

            Operator op = opstack.back() .GetToken() .op;
            opstack.pop_back() ;

            prevData = program.Operate( op, subject ) ;
            if ( prevData.type == ERROR ) 
              return false;
          } // while

          opstack.pop_back() ;
          
          /* 
          string debugstr = "\n(right-subject:";
          for ( int x = 0 ; x < subject.size() ; x ++ ) 
            debugstr += subject[x].GetString() + " , " ;
          debugstr += ")\n";
          debug( debugstr ) ;
          */ 

          if ( token.str == ")" ) {
            prevData = subject.back() ;
            
            Data temp = subject.size() >= 2 ? subject[subject.size() - 2] : Data() ;
            if ( temp.type == FUNCTION ) {
              prevData = program.OperateFunction( subject ) ;
            } // if


            if ( operation.mode == CONTROLFLOW_OPERATION && opstack.empty() ) 
              controlflow_delayed = true;
          } // if

          
          else if ( token.str == "]" ) {
            Operator op = Operator( "[]", 2 ) ;
            Token tempt = Token( op ) ;
            Data tempd = Data( tempt ) ;
            operation.exp.push_back( tempd ) ;

            prevData = program.Operate( op, subject ) ;
          } // else if

        } // if


        else if ( token.str == "}" ) 
          EndRoutine( program ) ;
      } // else if

    } // else


    if ( token.str == ";" ) {
      if ( operation.mode == OPERATION ) {
        debug( "\nend:(opstack:{ " ) ;
        for ( int x = 0 ; x < opstack.size() ; x ++ ) 
          debug( "'" + opstack[x].GetToken() .str + "' " ) ;
        debug( "})" ) ;
        debug( "(subject:{ " ) ;
        for ( int x = 0 ; x < subject.size() ; x ++ ) 
          debug( subject[x].GetString() + " " ) ;
        debug( "})\n" ) ;

        if ( opstack.size() > 1 ) {
          prevData = Data( "unexpected token : '" + token.str + "'", ERROR ) ;
          return false;
        } // if


        /* 
        if ( subject.size() > 1 ) {
          prevData = Data( "unexpected token : '" + token.str + "'", ERROR ) ;
          return false;
        } // if

        */ 

        if ( subject.size() == 1 && ! subject.back() .IsOperand() && subject.back() .type != VOID ) {
          debug( "(this)" ) ;
          prevData = Data( "unexpected token : '" + token.str + "'", ERROR ) ;
          return false;
        } // if

      } // if


      AppendOperation( program ) ;
    } // if

    return true;
  } // SemanticAnalyze()


  void GetChar() {
    debug( "(char)" ) ;
    if ( index >= len ) 
      return;

    token.str = "'";

    if ( code[index] == '\n' ) 
      return;

    token.str += code[index];

    if ( ++ index >= len ) 
      return;

    if ( code[index] == '\n' ) 
      return;

    if ( code[index ++ ] == '\'' ) {
      token.str += "'";
      token.type = CHAR;
    } // if

      
    debug( "(char-end:" + token.str + ")" ) ;
  } // GetChar()


  void GetString() {
    if ( index >= len ) 
      return;

    token.str = '"';
    char c = code[index];

    while ( c != '"' && c != '\n' ) {
      if ( c == 'n' && ! token.str.empty() && token.str[token.str.size() - 1] == '\\' ) 
        token.str[token.str.size() - 1] = '\n';
      else
        token.str.push_back( c ) ;

      if ( ++ index >= len ) 
        return;

      c = code[index];
    } // while


    if ( code[index] == '\n' ) 
      return;

    if ( code[index ++ ] == '"' ) {
      token.str += '"';
      token.type = STRING;
    } // if

  } // GetString()


  void Commented() {
    debug( "(commented)\n" ) ;
    -- index;
    while ( index < len && code[index ++ ] != '\n' ) ;
    token.Reset() ;
  } // Commented()

  
  void SetError( string str, Program & program ) {
    operation.exp.clear() ;
    operation.mode = ERROR_OPERATION;
    str = "Line " + to_string( line ) + " : " + str;
    Data error = Data( str, ERROR ) ;
    operation.exp.push_back( error ) ;

    if ( program.IsMain() ) 
      program.subroutines.back() .operations.push_back( operation ) ;
    
    else {
      while ( ! program.IsMain() ) 
        program.subroutines.pop_back() ;

      program.subroutines.back() .operations.back() = operation;
    } // else


    ResetOperation( program ) ;
    parentheses.clear() ;
    Commented() ;
  } // SetError()


  void AppendOperation( Program & program ) {
    debug( "(append-operation)" ) ;
    program.subroutines.back() .operations.push_back( operation ) ;

    Token tempt = Token( "\n", NONE ) ;
    tokens.push_back( tempt ) ;
    tokens_layer.push_back( 0 ) ;

    if ( ! program.IsMain() ) {
      if ( operation.mode == VARIABLE_DECLARATION ) 
        program.VariableDeclaration( operation.exp, false ) ;

      if ( parentheses.back() != "{" ) 
        EndRoutine( program ) ;
    } // if


    ResetOperation( program ) ;
  } // AppendOperation()


  void AppendSubroutine( Program & program, string scope ) {
    Data temp = Data( "", SUBROUTINE ) ;
    operation.exp.push_back( temp ) ;
    program.subroutines.back() .operations.push_back( operation ) ;
    Routine emptyRoutine = Routine() ;
    program.subroutines.push_back( emptyRoutine ) ;
    parentheses.push_back( scope ) ;

    int layer = program.subroutines.size() ;
    debug( "(append-subroutine:" + to_string( layer ) + ")" ) ;

    Token tempt;

    tempt = Token( "\n", NONE ) ;
    tokens.push_back( tempt ) ;
    tokens_layer.push_back( layer ) ;

    if ( operation.mode == FUNCTION_DECLARATION ) 
      FunctionDeclarationExamine( program, operation.exp ) ;

    ResetOperation( program ) ;
  } // AppendSubroutine()


  void FunctionDeclarationExamine( Program & program, vector < Data > & exp ) {
    debug( "(semantic-function-declaration)" ) ;
    DataType funcDataType = string_to_dataType( exp[0].GetToken() .str ) ;
    string funcName = exp[1].GetToken() .str;
    vector < Parameter > params;
    Parameter param;

    debug( "(a)" ) ;
    vector < Data > var_dec_exp;
    string var;
    string routineName;
    bool is_reference;

    for ( int k = 2 ; k < exp.size() - 1 ; k ++ ) {
      var_dec_exp.push_back( exp[k] ) ;
      if ( exp[ ++ k].GetToken() .str == "&" ) {
        is_reference = true;
        ++ k;
      } // if

      var_dec_exp.push_back( exp[k] ) ;
      program.VariableDeclaration( var_dec_exp, false ) ;
      var_dec_exp.clear() ;

      param = Parameter( exp[k].data[0].str, is_reference ) ;
      params.push_back( param ) ;
    } // for

  } // FunctionDeclarationExamine()


  string GetFuncShowup( Program & program, vector < Token > & tokens, vector < int > & tokens_layer ) {
    debug( "(get-function-list-string)" ) ;
    if ( tokens.size() != tokens_layer.size() ) 
      debug( "(!!!!!!!!!!!!!)" ) ;

    string showup = "";
    int amount;
    bool leading_word = true;

    /* 
    debug( "\n(func-tokens:{ " ) ;
    for ( int k = 0 ; k < tokens.size() ; k ++ ) 
      debug( "'" + tokens[k].str + "' " ) ;
    debug( "})\n" ) ;
    debug( "(token-size:" + to_string( ( int ) tokens.size() ) + ")\n" ) ;
    */ 

    // for(int k = 0 ; k < 10 ; k++){
    showup += tokens[0].str + " ";
    for ( int k = 1 ; k < tokens.size() ; k ++ ) {
      if ( tokens[k].str == "}" ) {
        amount = ( tokens_layer[k] - 2 ) * 2;
        if ( amount > 0 ) 
          showup += string( amount, ' ' ) ;
        showup += "}\n";
        leading_word = true;
      } // if

      else {
        if ( leading_word ) {
          leading_word = false;
          amount = ( tokens_layer[k] - 1 ) * 2;
          if ( amount > 0 ) 
            showup += string( amount, ' ' ) ;
        } // if


        if ( tokens[k].type == SYMBOL && ( tokens[k - 1].str == "++" || tokens[k - 1].str == "--" ) ) 
          showup.erase( showup.end() - 1 ) ;

        if ( tokens[k].str == "[" || tokens[k].str == "," ) 
          showup.erase( showup.end() - 1 ) ;

        if ( ( tokens[k].str == "++" || tokens[k].str == "--" ) && tokens[k - 1].type == SYMBOL ) 
          showup.erase( showup.end() - 1 ) ;

        if ( tokens[k].str == ")" && tokens[k - 1].str == "(" ) 
          showup.erase( showup.end() - 1 ) ;

        if ( tokens[k].str == "(" && tokens[k - 1].type == SYMBOL ) 
          showup.erase( showup.end() - 1 ) ;

        if ( tokens[k].str == "\n" ) {
          showup.erase( showup.end() - 1 ) ;
          leading_word = true;
        } // if


        if ( tokens[k].type == STRING ) 
          showup += "\"" + tokens[k].str + "\"";
        else if ( tokens[k].type == CHAR ) 
          showup += "'" + tokens[k].str + "'";
        else
          showup += tokens[k].str;

        if ( tokens[k].str != "\n" ) 
          showup.push_back( ' ' ) ;
      } // else

    } // for


    // debug("\nshowup:\n" + showup + "\n");
    return showup;
  } // GetFuncShowup()


  void EndRoutine( Program & program ) {
    string routine_name;
    do {
      int ct = program.subroutines.size() ;
      debug( "(end-subroutine:" + to_string( ct ) + ")" ) ;
      routine_name = "R" + to_string( routineID ++ ) ;
      program.subroutines.back() .variables.clear() ;
      program.Routines[routine_name] = program.subroutines.back() ;
      program.subroutines.pop_back() ;
      parentheses.pop_back() ;
      
      program.subroutines.back() .operations.back() .exp.back() .data.back() .str = routine_name;

      string header = program.subroutines.back() .operations.back() .exp[0].GetToken() .str;
      if ( header == "if" || header == "else if" ) {
        debug( "(delayed)" ) ;
        delayed = true;
        return;
      } // if

    }  while ( ! program.IsMain() && parentheses.back() != "{" ) ;

    ResetOperation( program ) ;
  } // EndRoutine()


  void ResetOperation( Program & program ) {
    debug( "(reset-operation)" ) ;
    if ( ! delayed && program.IsMain() ) {
      debug( "(pushed)" ) ;
      pushed = line_reset = true;

      if ( program.subroutines.back() .operations.back() .mode == FUNCTION_DECLARATION ) {
        string funcName = program.subroutines.back() .operations.back() .exp[1].GetToken() .str;
        debug( "(set-func:" + funcName + ")" ) ;
        string showup = GetFuncShowup( program, tokens, tokens_layer ) ;
        Data temp = Data( showup, NONE ) ;
        program.subroutines.back() .operations.back() .exp.push_back( temp ) ;
      } // if


      tokens.clear() ;
      tokens_layer.clear() ;
    } // if


    token.Reset() ;
    data.Reset() ;
    prevData.Reset() ;
    operation.Reset() ;

    opstack.clear() ;
    subject.clear() ;
  } // ResetOperation()

} ;

class Interpreter {
public : 
  Program program;
  Parser parser;

  void Execute( string & code ) {
    parser.Setup( code ) ;

    while ( parser.Parse( program ) ) {

      if ( ! program.Execute() ) 
        return;
    } // while

  } // Execute()

} ;




string gtestNumber;

int main() {
  // freopen("testcases/input/b.txt","r", stdin);
  // freopen("testcases/input/test.txt","r", stdin);
  // freopen( "testcases/input/p2-14-3.txt", "r", stdin ) ;
  // freopen("output.txt", "w", stdout);
  
  Interpreter interpreter;
  char c;
  string code;
  gtestNumber = "";

  cout << "Our-C running ...\n";

  while ( scanf( "%c", & c ) != EOF && c != '\n' ) 
    gtestNumber.push_back( c ) ;

  while ( scanf( "%c", & c ) != EOF ) 
    code.push_back( c ) ;

  interpreter.Execute( code ) ;
  cout << "Our-C exited ...\n";

  return 0;
} // main()






string TO_STRING( const int num ) {
  char str[50];
  sprintf( str, "%d", num ) ;
  return string( str ) ;
} // TO_STRING()


string TO_STRING( const double num ) {
  char str[50];
  sprintf( str, "%.3f", num ) ;
  return string( str ) ;
} // TO_STRING()


int STOI( const string & str ) {
  return atoi( str.c_str() ) ;
} // STOI()


float STOF( const string & str ) {
  return atof( str.c_str() ) ;
} // STOF()


bool CHAR_IS_SYMBOL( const char & c ) {
  return ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ) || c == '_' || ( '0' <= c && c <= '9' ) ;
} // CHAR_IS_SYMBOL()


bool CHAR_IS_NUMBER( const char & c ) {
  return '0' <= c && c <= '9';
} // CHAR_IS_NUMBER()


bool IS_FLOAT( const string & str ) {
  if ( str == "+." || str == "-." ) 
    return false;

  int x = 0, l = str.size() ;

  for ( x = ( str[0] == '+' || str[0] == '-' ) ; str[x] != '.' && x < l ; x ++ ) 
    if ( str[x] < '0' || str[x] > '9' ) 
      return false;

  for ( ++ x ; x < l ; x ++ ) 
    if ( str[x] < '0' || str[x] > '9' ) 
      return false;

  return true;
} // IS_FLOAT()


bool IS_WHITESPACE( const char & c ) {
  return ( c == ' ' || c == '\t' || c == '\n' ) ;
} // IS_WHITESPACE()


DataType STRING_TO_DATATYPE( string str ) {
  return str == "bool"   ? BOOLEAN : 
      str == "char"   ? CHAR : 
      str == "string" ? STRING : 
      str == "int"    ? INT : 
      str == "float"  ? FLOAT : 
      str == "void"   ? VOID : NONE;
} // STRING_TO_DATATYPE()


void DEBUG( string str ) {
  if ( gtestNumber == "3" ) 
    cout << "";
} // DEBUG()


void DEBUGY( string str ) {
  if ( gtestNumber == "3" ) 
    cout << "";
} // DEBUGY()

