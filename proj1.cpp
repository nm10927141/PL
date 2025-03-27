# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <string>
# include <vector>
# include <cmath>
# include <map>

using namespace std;

string TO_STRING( const int & num ) ;
string TO_STRING( const double & num ) ;

int   STOI( const string & str ) ;
double STOF( const string & str ) ;

bool CHAR_IS_SYMBOL( const char & c ) ;
bool CHAR_IS_NUMBER( const char & c ) ;

bool IS_INTEGER( const string & str ) ;
bool IS_FLOAT( const string & str ) ;
bool IS_WHITESPACE( const char & c ) ;
bool IS_OPERATOR( const string & str ) ;
bool IS_VARIABLE( const string & str ) ;

void PRINT_UNEXPECTED( const string & str ) ;
void PRINT_UNRECOGNIZED( const string & str ) ;
bool FLOAT_FORMAT_EXCEPTION( const string & str ) ;

# define to_string TO_STRING

# define stoi       STOI
# define stof       STOF

# define char_is_symbol         CHAR_IS_SYMBOL
# define char_is_number         CHAR_IS_NUMBER

# define is_symbol              IS_SYMBOL
# define is_integer             IS_INTEGER
# define is_float               IS_FLOAT
# define is_whitespace          IS_WHITESPACE
# define is_operator            IS_OPERATOR
# define is_variable            IS_VARIABLE
# define print_unexpected       PRINT_UNEXPECTED
# define print_unrecognized     PRINT_UNRECOGNIZED
# define float_format_exception FLOAT_FORMAT_EXCEPTION
# define TokenVector  gTokenVector
# define Variables    gVariables
# define Operators    gOperators

# define str      m_str
# define priority m_priority
# define rightass m_rightass
# define parser    m_parser
# define evaluator m_evaluator
# define operands m_operands
# define code            m_code
# define tokens          m_tokens
# define token           m_token
# define index           m_index
# define len             m_len
# define parentheseLevel m_parentheseLevel

class Operator {
public :
  string str;
  int    priority;
  bool   rightass; // default are left to right associative, else are inversed

  Operator() {
    str = "";
    priority = - 1;
    rightass = false;
  } // Operator()


  Operator( string s, int p ) {
    str = s;
    priority = p;
    rightass = false;
  } // Operator()


  Operator( string s, int p, bool right_associative ) {
    str = s;
    priority = p;
    rightass = right_associative;
  } // Operator()

} ;


enum DataType {
  NONE,
  ERROR,
  OPERATOR,
  SIGN,
  PARENTHESE,
  SYMBOL,
  BOOLEAN,
  INT,
  FLOAT,
  STRING,
  CHAR
} ;


# define type  m_type

# define set        SET
# define reset      RESET
# define is_operand IS_OPERAND

class Token {
public :
  string str;
  DataType type;

  Token() {
    reset() ;
  } // Token()


  Token( char & c ) {
    str = c;
    type = is_operator( str )     ? OPERATOR    :
        c == '(' || c == ')' ? PARENTHESE  :
        char_is_number( c )    ? INT         :
        char_is_symbol( c )    ? SYMBOL      :
        c == '.'             ? FLOAT       : NONE;
  } // Token()


  void SET( string & s, DataType tokenType ) {
    str = s;
    type = tokenType;
  } // SET()


  void RESET() {
    str = "";
    type = NONE;
  } // RESET()


  bool IS_OPERAND() {
    return type == SYMBOL || type == INT || type == FLOAT;
  } // IS_OPERAND()

} ;

# define parse             PARSE
# define syntaxAnalyze     SyntaxAnalyze
# define push_token        PUSHTOKEN
# define commented         COMMENTED
# define unrecognizedError UNRECOGNIZEDERROR
# define unexpectedError   UNEXPECTEDERROR
# define undefinedError UNDEFINEDERROR
# define printTokens       PRINTTOKENS

vector < Token >         gTokenVector;


class Parser {
public :
  string code;
  vector < Token > tokens;
  Token token;
  int index, len;
  int parentheseLevel;
  bool mBoolToken;

  void PARSE( string & inputCode ) {
    code.swap( inputCode ) ;
    tokens.resize( 1 ) ;
    len = code.size() ;
    parentheseLevel = 0;
    string copyCode = "";
    index = 0;
    for ( int i = 0 ; i < len ; i++ ) {
      if ( code[i] == '/' && ( i + 1 < len && code[i+1] == '/' ) ) {
        while ( code[i] != '\n' ) i++;
      }  // end if

      copyCode += code[i];
    }  // end for

    code = copyCode;
    tokens.clear();
  } // PARSE()


  int SyntaxAnalyze() {
    string temp;
    char c;
    mBoolToken = false;
    while ( index < len && IS_WHITESPACE(code[index] ) ) index ++;
      
    for ( ; index < len ; index ++ ) {
      c = code[index];
      // cout << c;

      if ( token.str == "" ) {  // 如果現在什麼都沒有
        if ( ! is_whitespace( c ) ) {  // 目前的char 不是 white spate
          token = Token( c ) ;  //  先用這個char 建立一個token

          if ( index + 1 < len ) {  // 如果還有下一個char
            // check +- is number or operator

            // 如果一個cmd的token list是空的，或
            // 現在的token是'+', '-' 且 token list的最後一個token的type是op
            if ( ( token.str == "+" || token.str == "-" ) && tokens.size() == 0 ) {
              token.type = SIGN;
            } // end if

            else if ( ( token.str == "+" || token.str == "-" ) &&
                      ( tokens.back().type == OPERATOR || tokens.back().str == "(" ) ) {
              token.type = SIGN;
            } // end else if

            // combine with next character to operator if it would be
            // 輸入還沒結束，且目前token不是op
            if ( index + 1 < len && ! token.is_operand() ) {
              // 先把目前的token string加入下一個char
              temp = token.str + code[index + 1];

              // 看這樣有沒有別的意義
              if ( is_operator( temp ) || temp == "//" ) {
                token.set( temp, OPERATOR ) ;
                index ++ ;
              } // if

            } // if

          } // if

          // 如果token什麼都不是也不是分號，代表unrecognized Error
          if ( token.type == NONE && token.str != ";" )
            return unrecognizedError() ;
            
          if ( token.str == ":=" ) {
            bool error = false;
            for ( int i = 0 ; i < tokens.size() && error == false ; i++ ) {
              if ( tokens[i].str == ":=" ) {
                unexpectedError() ;
                return -2;
              }  // end if
            }  // end for
          }  // end if
            
          // 如果token type 不是 SYMBOL, INT, FLOAT; 就 pushtoken。
          // pushtoken 會檢查有沒有語法錯誤
          else if ( ! ( token.is_operand() || token.type == SIGN ) ) {
            int result = push_token();
            if ( result != 0 ) return result;
          } // else if

        } // if

      } // if


      else {  // 如果現在token已經有東西了 但他還沒有被push token
        // 是Symbol
        if ( char_is_symbol( c ) && token.type == SYMBOL )
          token.str.push_back( c ) ;
        
        // 是 int or float
        else if ( char_is_number( c ) &&
                  ( token.type == SIGN || token.type == INT || token.type == FLOAT ) ) {
          if ( token.type == SIGN ) token.type = INT;
          token.str.push_back( c ) ;
        }  // end else if

        // 是 float
        else if ( c == '.' && ( token.type == SIGN || token.type == INT ) ) {
          token.str.push_back( c ) ;
          if ( token.type == SIGN ) {
            int i = 1;
            while ( is_whitespace( code[len + i] ) ) i++;
            if ( char_is_number( code[len + i] ) ) token.type = FLOAT;
            else token.type = ERROR;
          }  // end if

          else token.type = FLOAT;
        } // else if

        else if ( is_whitespace( c ) && token.type == SIGN ) ;
          
        // 什麼都不是 ， push token
        else {
          int result = push_token();
          if ( result != 0 ) return result;
          // 因為他已經不匹配了，所以要減一，讓他去看他是什麼東西
          -- index;
        } // else

      } // else

    } // for


    if ( token.type != NONE ) {
      int result = push_token();
      if ( result != 0 ) return result;
    } // if

    return 1;
  } // SyntaxAnalyze()


  int PUSHTOKEN() {
    bool error = false;
    Token prevToken;
    string str = token.str;
    if ( str == "=" || str == "<>" || str == "<" || str == ">" || str == "<=" || str == ">=" ) {
      if ( mBoolToken == false ) mBoolToken = true;
      else return unexpectedError() ;
    }  // end if

    // for(int y = 0 ; y < tokens.back().size() ; y++)
    //    cout <<"'"<< tokens.back()[y].str <<"'"<<" ";
    // cout << "<< '" <<  token.str << "'\n";

    if ( ! tokens.empty() )
      prevToken = tokens.back() ;
    else
      prevToken.reset() ;

    if ( token.str == ":=" ) {
      for ( int i = 0 ; i < tokens.size() ; i++ ) {
        if ( tokens[i].str == ":=" ) {
          return unexpectedError() ;
        }  // end if
      }  // end for
    }  // end if

    if ( tokens.size() == 0 ) ;
    
    else if ( tokens.size() == 1 ) {
      if ( token.str == ":=" ) ;  // 不用檢查第一個有沒有被定義
        
      else if ( prevToken.type == SYMBOL && token.type == SYMBOL ) {  // 要檢查第一個有沒有被定義
        return unexpectedError();
      }  // end else if
        
      else if ( prevToken.type == SYMBOL ) {  // 要檢查第一個有沒有被定義
        if ( ! is_variable( prevToken.str ) ) {
          token = prevToken;
          return undefinedError();
        }  // end if
      }  // end else if
    }  // end if

    else if ( token.type == SYMBOL && tokens.size() > 1 ) {
      if ( ! is_variable( token.str ) ) return undefinedError();
    }  // end else if

    // Unexpected token: X (
    if ( token.str == "(" && prevToken.type != OPERATOR && prevToken.str != "" && prevToken.str != "(" )
      return unexpectedError() ;

    // Unexpected token: X )
    if ( token.str == ")" && ! prevToken.is_operand() && prevToken.str != ")" )
      return unexpectedError() ;

    // Unexpected token: parentheses level
    if ( token.type == PARENTHESE ) {
      parentheseLevel += ( token.str == "(" ? 1 : - 1 ) ;
      if ( parentheseLevel < 0 )
        return unexpectedError() ;
    } // if

    if ( ( ! ( token.type == INT || token.type == FLOAT ) && prevToken.type == SIGN ) )
      return unexpectedError() ;

    // Unexpected token: continuous operator
    if ( token.type == OPERATOR && prevToken.type == OPERATOR )
      return unexpectedError() ;
      
    // Unexpected token: continuous symbol or number tokens
    if ( token.is_operand() && prevToken.is_operand() )
      return unexpectedError() ;

    // Functional token
    if ( token.str == ";" ) {
      if ( tokens.empty() ) {
        return unexpectedError() ;
      } // if
        
      else if ( parentheseLevel != 0 ) return unexpectedError();

      index++;
      token.reset() ;
      return 2;
    } // if


    if ( token.str == "quit" && tokens.empty() ) {
      token.str = "Program exits...";
      token.type = ERROR;
      tokens.push_back( token ) ;
      return 1;
    } // if


    // Expected
    tokens.push_back( token ) ;
    token.reset() ;


    // cout << "dddd\n";

    return 0;
  } // PUSHTOKEN()


  void COMMENTED() {
    token.reset() ;
    while ( index < len && code[index] != '\n' )
      index ++ ;
  } // COMMENTED()


  int UNRECOGNIZEDERROR() {
    token.str = "Unrecognized token with first char : '" + token.str.substr( 0, 1 ) + "'";
    token.type = ERROR;
    tokens.push_back( token ) ;
    parentheseLevel = 0;
    commented() ;
    return - 1;
  } // UNRECOGNIZEDERROR()


  int UNEXPECTEDERROR() {
    token.str = "Unexpected token : '" + token.str + "'";
    token.type = ERROR;
    tokens.push_back( token ) ;
    parentheseLevel = 0;
    commented() ;
    return - 2;
  } // UNEXPECTEDERROR()

  int UNDEFINEDERROR() {
    token.str = "Undefined identifier : '" + token.str + "'";
    token.type = ERROR;
    tokens.push_back( token ) ;
    parentheseLevel = 0;
    commented() ;
    return - 3;
  } // UNDEFINEDERROR()




  // void PRINTTOKENS() {
  //   int x, y;
  //   int xl, yl;

  //   xl = tokens.size() ;
  //   for ( x = 0 ; x < xl ; x ++ ) {
  //     yl = tokens[x].size() ;
  //     for ( y = 0 ; y < yl ; y ++ )
  //       cout << "'" << tokens[x][y].str << "' ";
  //     cout << '\n';
  //   } // for

  // } // PRINTTOKENS()

} ;

# define i m_i
# define f m_f

# define printData PRINTDATA

 // Handle of Token & Data processing
struct Data{
  DataType type;
  string str;
  int i;
  double f;
};

typedef Data* DataPtr;

DataPtr NewData() {
  DataPtr data = new Data();
  data->type = NONE;
  data->str = "";
  data->i = 0;
  data->f = 0;
  return data;
}  // end NewData()

DataPtr NewData( DataType type, string str, int i, double f ) {
  DataPtr data = new Data();
  data->type = type;
  data->str = str;
  data->i = i;
  data->f = f;
  return data;
}  // end NewData()

DataPtr NewData( Token token ) {
  DataPtr data = new Data();
  data->type = token.type;

  if ( data->type == INT )
    data->f = data->i = stoi( token.str ) ;
  else if ( data->type == FLOAT )
    data->i = data->f = stof( token.str ) ;
  
  data->str = token.str;
  return data;
} // NewData()


DataPtr NewData( string op ) {
  DataPtr data = new Data();
  data->type = OPERATOR;
  data->str = op;
  return data;
} // NewData()


DataPtr NewData( double val ) {
  DataPtr data = new Data();
  data->type = FLOAT;
  data->i = data->f = val;
  return data;
} // NewData()


DataPtr NewData( int val ) {
  DataPtr data = new Data();
  data->type = INT;
  data->f = data->i = val;
  return data;
} // NewData()


DataPtr NewData( bool val ) {
  DataPtr data = new Data();
  data->type = BOOLEAN;
  data->f = data->i = val;
  return data;
} // NewData()


bool IS_OPERAND( DataPtr data ) {
  return data->type == SYMBOL || data->type == INT || data->type == FLOAT;
} // IS_OPERAND()

void PRINTDATA( DataPtr data ) {
  if ( data->type == BOOLEAN )
    cout << ( data->i == 1 ? "true" : "false" ) ;
  else if ( data->type == INT )
    cout << data->i;
  else if ( data->type == FLOAT ) {
    // cout << "float : ";
    cout << fixed << setprecision( 3 ) << data->f;
  } // else if


  else if ( data->type == ERROR ) {
    if ( data->i == - 1 )
      cout << "Unexpected token : ':='";
    else
      cout << "Undefined identifier : '" << data->str << "'";
  } // else if

} // PRINTDATA()

map < string, DataPtr >     gVariables;
map < string, Operator > gOperators;

# define push_operand       PUSH_OPERAND
# define operate            OPERATE
# define resolve_operand_2  RESOLVEOPRD2
# define resolve_operand    RESOLVEOPRD
# define operand_2          OPERAND2
# define mul    MUL
# define div    DIV
# define add    ADD
# define sub    SUB
# define gt     GT
# define lt     LT
# define ge     GE
# define le     LE
# define eq     EQ
# define neq    NEQ
# define assign ASSIGN


class Evaluator {
public :
  vector < DataPtr > operands;

  void RESET() {
    operands.clear() ;
  } // RESET()


  void PUSH_OPERAND( DataPtr & operand ) {
    operands.push_back( operand ) ;
  } // PUSH_OPERAND()

  
  void OPERATE( string & op ) {
    DataPtr result = op == "*"  ? mul( operand_2() ) :
           op == "/"  ? div( operand_2() ) :
           op == "+"  ? add( operand_2() ) :
           op == "-"  ? sub( operand_2() ) :
           op == ">"  ? gt( operand_2() )  :
           op == "<"  ? lt( operand_2() )  :
           op == ">=" ? ge( operand_2() )  :
           op == "<=" ? le( operand_2() )  :
           op == "="  ? eq( operand_2() )  :
           op == "<>" ? neq( operand_2() ) :
           op == ":=" ? assign( operand_2() ) : NewData() ;

    operands.push_back( result ) ;
  } // OPERATE()



  pair < DataPtr, DataPtr > OPERAND2() {
    pair < DataPtr, DataPtr > result;

    result.second = operands.back() ;
    operands.pop_back() ;
    result.first = operands.back() ;
    operands.pop_back() ;

    return result;
  } // OPERAND2()


  bool RESOLVEOPRD( DataPtr & operand ) {
    if ( operand->type == SYMBOL ) {
      if ( is_variable( operand->str ) ) {
        DataPtr temp = Variables[operand->str];
        operand = NewData( temp->type, temp->str, temp->i, temp->f );
      }  // end if
      
      else {
        operand->type = ERROR;
        return false;
      } // else

    } // if


    return true;
  } // RESOLVEOPRD()


  bool RESOLVEOPRD2( pair < DataPtr, DataPtr > & param ) {
    return resolve_operand( param.first ) && resolve_operand( param.second ) ;
  } // RESOLVEOPRD2()


  DataPtr MUL( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i * param.second->i ) ;
    
    return NewData( param.first->f * param.second->f ) ;
  } // MUL()


  DataPtr DIV( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i / param.second->i ) ;

    return NewData( param.first->f / param.second->f ) ;
  } // DIV()


  DataPtr ADD( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i + param.second->i ) ;

    return NewData( param.first->f + param.second->f ) ;
  } // ADD()


  DataPtr SUB( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i - param.second->i ) ;

    return NewData( param.first->f - param.second->f ) ;
  } // SUB()


  DataPtr GT( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i > param.second->i ) ;
    
    if (  param.first->f >= param.second->f + 0.0001 )
      return NewData( true ) ;
        
    else return NewData( false );
  } // GT()


  DataPtr LT( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i < param.second->i ) ;

    if (  param.first->f + 0.0001 <= param.second->f )
      return NewData( true ) ;
        
    else return NewData( false );
  } // LT()


  DataPtr GE( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i >= param.second->i ) ;

    return NewData( param.first->f >= param.second->f ) ;
  } // GE()


  DataPtr LE( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i <= param.second->i ) ;

    return NewData( param.first->f <= param.second->f ) ;
  } // LE()


  DataPtr EQ( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i == param.second->i ) ;

    if ( abs( param.first->f - param.second->f ) < 0.0001 )
      return NewData( true ) ;
    
    else return NewData( false );
  } // EQ()


  DataPtr NEQ( pair < DataPtr, DataPtr > param ) {
    if ( ! resolve_operand_2( param ) )
      return param.first->type == ERROR ? param.first : param.second;

    if ( param.first->type == INT && param.second->type == INT )
      return NewData( param.first->i != param.second->i ) ;

    if ( abs( param.first->f - param.second->f ) >= 0.0001 )
      return NewData( true ) ;
    
    else return NewData( false );
  } // NEQ()


  DataPtr ASSIGN( pair < DataPtr, DataPtr > param ) {
    if ( param.first->type != SYMBOL ) {
      param.first->type = ERROR;
      param.first->i = - 1;
      return param.first;
    } // if


    if ( ! resolve_operand( param.second ) )
      return param.second;

    return Variables[param.first->str] = param.second;
  } // ASSIGN()

} ;

# define execute   EXECUTE
# define suffix_expression Suffix_expression
# define evaluate  EVALUATE
# define printExpression PrintExpression

class Interpreter {
public :
  Parser parser;
  Evaluator evaluator;

  void EXECUTE( string & code ) {
    parser.parse( code ) ;
    // parser.printTokens();

    while ( true ) {
      if ( parser.index >= parser.len ) {
        cout << "Program exits..." ;
        exit(0);
        return ;
      }  // end if

      cout << "> ";
      int result = parser.SyntaxAnalyze();
      if ( parser.tokens.back() .type == ERROR ) {
        cout << parser.tokens.back() .str;
        if ( result == 1 ) {
          return;
        } // end if
      }  // end if

      else {
        vector < DataPtr > exp = suffix_expression( parser.tokens ) ;
        // printExpression(exp);
        printData( evaluate( exp ) ) ;
      } // else

      cout << "\n";
      parser.token.reset();
      parser.tokens.clear();
    } // while()

  } // EXECUTE()


  vector < DataPtr > Suffix_expression( vector < Token > & tokens ) { // make suffix expression
    int x, l = tokens.size() ;
    Token token;
    Operator op;
    vector < DataPtr > buffer;
    vector < DataPtr > expression;
    vector < Operator > opstack;
    DataPtr tempData;
    
    for ( x = 0 ; x < l ; x ++ ) {
      token = tokens[x];

      if ( token.is_operand() ) {
        tempData = NewData( tokens[x] ) ;
        expression.push_back( tempData ) ;
      } // if

      else {
        if ( token.str == "(" ) {
          op = Operator( "(", 99 ) ;
          opstack.push_back( op ) ;
        } // if

        
        else if ( token.str == ")" ) {
          while ( ! opstack.empty() && opstack.back() .str != "(" ) {
            tempData = NewData( opstack.back() .str ) ;
            expression.push_back( tempData ) ;
            opstack.pop_back() ;
          } // while


          opstack.pop_back() ;
        } // else if

        else if ( token.type == OPERATOR ) {
          op = Operators[token.str];
          
          while ( ! opstack.empty() && op.priority >= opstack.back() .priority ) {
            tempData = NewData( opstack.back() .str ) ;
            expression.push_back( tempData ) ;
            opstack.pop_back() ;
          } // while


          opstack.push_back( op ) ;
        } // else if

      } // else

    } // for


    while ( ! opstack.empty() ) {
      tempData = NewData( opstack.back() .str ) ;
      expression.push_back( tempData ) ;
      opstack.pop_back() ;
    } // while


    return expression;
  } // Suffix_expression()

  DataPtr EVALUATE( vector < DataPtr > & exp ) {
    string op;
    evaluator.reset() ;
      
    while ( ! exp.empty() ) {
      if ( is_operand( exp.front() ) )
        evaluator.push_operand( exp.front() ) ;
      else
        evaluator.operate( exp.front()->str ) ;

      exp.erase( exp.begin() ) ;
    } // while

    if ( evaluator.operands.front()->type != ERROR )
      evaluator.resolve_operand( evaluator.operands.front() );

    return evaluator.operands.front() ;
  } // EVALUATE()
} ;


void TableSetup() {
  Operators[ "*"]  = Operator( "*",  5 ) ;
  Operators[ "/"]  = Operator( "/",  5 ) ;

  Operators[ "+"]  = Operator( "+",  6 ) ;
  Operators[ "-"]  = Operator( "-",  6 ) ;

  Operators[ ">"]  = Operator( ">",  9 ) ;
  Operators[ "<"]  = Operator( "<",  9 ) ;
  Operators[ ">="] = Operator( ">=", 9 ) ;
  Operators[ "<="] = Operator( "<=", 9 ) ;

  Operators[ "="]  = Operator( "=",  10 ) ;
  Operators[ "<>"] = Operator( "<>", 10 ) ;

  Operators[ ":="] = Operator( ":=", 17, true ) ;
} // TableSetup()






int main() {
  // freopen( "test.txt", "r", stdin ) ;
  // freopen("output.txt", "w", stdout);
  
  Interpreter interpreter;
  char c;
  string code;
  string testNumber;

  TableSetup() ;
  cout << "Program starts...\n";

  while ( scanf( "%c", & c ) != EOF && c != '\n' )
    testNumber.push_back( c ) ;

  while ( scanf( "%c", & c ) != EOF )
    code.push_back( c ) ;

  interpreter.execute( code ) ;
} // main()






string TO_STRING( const int & num ) {
  char str[50];
  sprintf( str, "%d", num ) ;
  return string( str ) ;
} // TO_STRING()


string TO_STRING( const double & num ) {
  char str[50];
  sprintf( str, "%.3f", num ) ;
  return string( str ) ;
} // TO_STRING()


int STOI( const string & str ) {
  return atoi( str.c_str() ) ;
} // STOI()


double STOF( const string & str ) {
  return atof( str.c_str() ) ;
} // STOF()


bool CHAR_IS_SYMBOL( const char & c ) {
  return ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' )
      || c == '_' || ( '0' <= c && c <= '9' ) ;
} // CHAR_IS_SYMBOL()


bool CHAR_IS_NUMBER( const char & c ) {
  return '0' <= c && c <= '9';
} // CHAR_IS_NUMBER()


bool IS_SYMBOL( const string & str ) {
  char c;

  if ( '0' <= str[0] && str[0] <= '9' )
    return false;

  for ( int x = 0, l = str.size() ; x < l ; x ++ ) {
    c = str[x];
    if ( ! ( ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' )
             || c == '_' || ( '0' <= c && c <= '9' ) ) )
      return false;
  } // for


  return true;
} // IS_SYMBOL()


bool IS_INTEGER( const string & str ) {
  int x = 0, l = str.size() ;

  for ( x = ( str[0] == '+' || str[0] == '-' ) ; x < l ; x ++ )
    if ( str[x] < '0' || str[x] > '9' )
      return false;

  return true;
} // IS_INTEGER()


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


bool IS_OPERATOR( const string & str ) {
  return Operators.find( str ) != Operators.end() ;
} // IS_OPERATOR()


bool IS_VARIABLE( const string & str ) {
  return Variables.find( str ) != Variables.end() ;
} // IS_VARIABLE()


void PRINT_UNEXPECTED( const string & str ) {
  cout << "Unexpected token : '" << str << "'";
} // PRINT_UNEXPECTED()


void PRINT_UNRECOGNIZED( const string & str ) {
  cout << "Unrecognized token with first char : '" << str[0] << "'";
} // PRINT_UNRECOGNIZED()

