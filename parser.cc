/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>


#include "parser.h"


using namespace std;
map<string,int> variables;
vector<int> inputs;
int next_location = 0;


void Parser::syntax_error()
{
    cout << "SYNTAX ERROR !&%!\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;
    
    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;
        token = lexer.peek(i);
        token.Print();
    }
   
    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }

}
/*------------------------------------------------------------------------*/
void Parser::parse_input(){
    parse_program(); // The whole program
    parse_inputs(); // actual inputs ie {1 4 3 4 5}
    expect(END_OF_FILE);
}

void Parser::parse_program(){
    parse_poly_decl_section();
    parse_START();
}

void Parser::parse_poly_decl_section(){
    parse_poly_decl();

    Token t = lexer.peek(1);
    if (t.token_type==POLY)
    {
        parse_poly_decl_section();
    }
}
void Parser::parse_poly_decl(){
    expect(POLY);
    parse_poly_header();
    expect(EQUAL);
    parse_poly_body();
    expect(SEMICOLON);
}
void Parser::parse_START(){
    expect(START);
    parse_statement_list();

}
vector<string> Parser::parse_poly_header(){ // F(args)
    vector<string> theIDS;

    parse_poly_name();
    Token t = lexer.peek(1);
    if (t.token_type == LPAREN)
    {
        expect(LPAREN);
        theIDS = parse_id_list();
        expect(RPAREN);
    }else if (t.token_type==EQUAL)
    {
        // if (theIDS.size()==0){ // MAYBE
        //     theIDS.push_back("x");
        // }
        return theIDS;
    }
    else
    {
        syntax_error();
    }
}
vector<string> Parser::parse_id_list(){
    vector<string> anID; //
    Token t = expect(ID);
    anID.push_back(t.lexeme); //
    t = lexer.peek(1);
    if (t.token_type == COMMA)
    {
        expect(COMMA);
        vector<string> restID = parse_id_list();
        anID.insert(anID.end(),restID.begin(),restID.end());
    }
    else if (t.token_type == RPAREN)
    {
        return anID;
    }
    else
    {
        syntax_error();
    }   
}
void Parser::parse_poly_name(){ // just gets ID of Poly, for instance Poly F, just gets F
    Token t = expect(ID);
}
void Parser::parse_poly_body(){ // just calls term list  down there.
    parse_term_list();
}

void Parser::parse_term_list(){ // parses {3x^2 +2x + 2 } where each monomials and +- are terms
    parse_term();
    Token t = lexer.peek(1);
    if (t.token_type  == PLUS ||t.token_type  == MINUS)
    {
        parse_add_operator();
        parse_term_list();
    }
    else if (t.token_type == SEMICOLON)
    {
        return;
    }
    else
    {
        syntax_error();
    }  
}
void Parser::parse_term(){ // Term will either be x ,  3x, 3x^2, x^2y
    Token t = lexer.peek(1);
    switch (t.token_type)
    {
    case ID:
        parse_monomial_list();
        break;
    case NUM:
        parse_coefficient();
        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            parse_monomial_list();
        }
        else
        {
            return;
        }
        break;
    
    default:
        syntax_error();
    }
}
void Parser::parse_monomial_list(){ // 3xyz the monomial list is xyz
    parse_monomial();
    Token t = lexer.peek(1);
    if (t.token_type==ID)
    {
        parse_monomial_list();
    }
    
}
struct Parser::monomial* Parser::parse_monomial(){ // 3x^2
    struct monomial *p = (struct monomial *)malloc(sizeof(struct monomial));
    Token t = expect(ID);
    // p->order = variables.at(t.lexeme);
    t = lexer.peek(1);
    if (t.token_type==POWER){
        p->exponent = parse_exponent();
    }
    
    return p;
}

void Parser::parse_inputs(){ // actually grabs all inputs ,  1 4 3 4 5
    storeInput(expect(NUM));
    Token t = lexer.peek(1);
    if (t.token_type==NUM)
    {
        parse_inputs();
    }
}
int Parser::parse_exponent(){ // ^2 , ^ is power , 2 is num
    expect(POWER);
    Token t = expect(NUM);
    return stoi(t.lexeme);
}
void Parser::parse_add_operator(){
    Token t = lexer.peek(1);
    if (t.token_type==PLUS)
    {
        expect(PLUS);
    }else if (t.token_type==MINUS)
    {
        expect(MINUS);
    }else
    {
        syntax_error();
    }
    
}
int Parser::parse_coefficient(){
    Token t = expect(NUM);
    return stoi(t.lexeme);
}
void Parser::parse_statement_list(){
    parse_statement();
    Token t = lexer.peek(1);
    if (t.token_type == ID || t.token_type == INPUT )
    {
        parse_statement_list();
    }
}
void Parser::parse_statement(){
    Token t = lexer.peek(1);
    switch (t.token_type)
    {
    case INPUT:
        parse_input_statement();
        break;
    case ID:
        parse_poly_eval_statement();
        break;
    default:
        syntax_error();
    }

}
void Parser::parse_poly_eval_statement(){ // F(args);
    parse_poly_eval();
    expect(SEMICOLON);
}
void Parser::parse_input_statement(){
    expect(INPUT);
    allocateMem(expect(ID));
    expect(SEMICOLON);
}
void Parser::parse_poly_eval(){  // F(args)
    parse_poly_name();
    expect(LPAREN);
    parse_arg_list();
    expect(RPAREN);
}
void Parser::parse_arg_list(){ // (x,y) x and y are the args
    parse_arg();
    Token t = lexer.peek(1);
    if (t.token_type==COMMA)
    {
        expect(COMMA);
        parse_arg_list();
    }
    
    
}
void Parser::parse_arg(){ // x, y inside parenthesis
    Token t = lexer.peek(1);
    switch (t.token_type)
    {
    case ID:
        t = lexer.peek(2);
        if (t.token_type==LPAREN) // I'm not sure about this. still passes though .
        {
            parse_poly_eval();
        }else
        {
            expect(ID);
        }break;
    case NUM:
        expect(NUM);
        break;
    default:
        parse_poly_eval();
        break;
    }
}
// Memory Allocation
void Parser::allocateMem(Token t){
    if (variables.find(t.lexeme)==variables.end()) // isn't present
    {
        variables.insert({t.lexeme,next_location++});
    }   
}
void Parser::storeInput(Token t){
    int num = stoi(t.lexeme);
    inputs.push_back(num);
}

/*------------------------------------------------------------------------*/
int main()
{
    
	Parser parser;
	// parser.ConsumeAllInput();
    parser.parse_input();
    
	
}

