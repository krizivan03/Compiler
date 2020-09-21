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
#include <set>
#include <algorithm>



#include "parser.h"


using namespace std;
Parser::param_ID *currentParams;
Parser::poly_dec *thePolyDeclarations;
Parser::stmt *theSMTS;
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


/*------------------------------------------------------------------------*/
void Parser::parse_input(){
    parse_program(); // The whole program
    parse_inputs(); // actual inputs ie {1 4 3 4 5}
    expect(END_OF_FILE);
}

void Parser::parse_program(){
    thePolyDeclarations = parse_poly_decl_section(); 
    Error_Code1(); Error_Code2();
    parse_START(); 
    Error_Code3(); 
    Error_Code4();
}
void Parser::Error_Code1(){
    Parser::poly_dec * p = thePolyDeclarations;
    map<string,int> temp;
    set<int> duplicates;
     
    while (p!=NULL)
    {
        if (temp.find(p->name)!=temp.end()) // is present
        {
            duplicates.insert(temp[p->name]);
            duplicates.insert(p->theToken.line_no);
            
        }else
        {
            string name = p->name;
            int lineNo = p->theToken.line_no;
            temp.insert({name,lineNo});
        }
        p = p->next_poly_dec;
    }
    if (duplicates.size()>0)
    {
     cout << "Error Code 1:";
     for (auto elem : duplicates)
        {
        cout << elem << " ";
        }
    }   
}
void Parser::Error_Code2(){
    vector<int> temp; // lineNo 
    Parser::poly_dec *p = thePolyDeclarations;
    
    while (p!=NULL)
    {
        Parser::param_ID *params = p->theparam_IDs;
        Parser::term *terms = p->terms;
        bool isValid = false;
        
        while (terms!=NULL)
        {
            Parser::monomial * m =terms->head_monomial;
            while (m!=NULL)
            {
                string suspect = m->theToken.lexeme;
                Parser::param_ID *i = params;
                while (i!=NULL)
                {
                    if (suspect==i->ID)
                    {
                        isValid = true; break;
                    }
                    
                    i = i->next;
                }
                if (isValid==false)
                {
                    temp.push_back(m->theToken.line_no);
                }
                isValid=false;
                m=m->next;
            }
            terms = terms->next;
        }
        p=p->next_poly_dec;
    }
    if (temp.size()>0)
    {
        cout << "Error Code 2: ";
     for (auto elem : temp)
        {
        cout << elem << " ";
        }
    }
}
void Parser::Error_Code3(){
    Parser::poly_dec *p =thePolyDeclarations;
    Parser::stmt *s = theSMTS;
    vector<string> *theDecs = new vector<string>; 
    vector<int> *lineNos = new vector<int>;

    while (p!=NULL) // theDecs
    {
        theDecs->push_back(p->name);
        p=p->next_poly_dec;
    }
    while (s!=NULL)
    {
        
        if (s->stmt_type == 1)
        {
            if (find(theDecs->begin(),theDecs->end(),s->theToken.lexeme)==theDecs->end()) // doesnt contain
            {
                lineNos->push_back(s->theToken.line_no);
            }   

            Parser::arg *a = s->p->theArgs;
            while (a!=NULL)
            {
                if (a->arg_type == POLY)
                {
                parse_EC3args(a,theDecs,lineNos);
                }
                a = a->next;
            }
            
            
        }
            s=s->next;
    }
    if (lineNos->size()>0)
    {
        cout << "Error Code 3: ";
     for (auto elem : *lineNos)
        {
        cout << elem << " ";
        }
        exit(1);
    }

}
void Parser::parse_EC3args(Parser::arg *a,vector<string> *d,vector<int> *l){
        while (a!=NULL)
        {
            if (a->arg_type == POLY){
                if (find(d->begin(),d->end(),a->theToken.lexeme)==d->end()) { // oesnt contain
                    l->push_back(a->theToken.line_no);
                }
                
                if (a->p->theArgs->arg_type == POLY)
                {
                    parse_EC3args(a->p->theArgs,d,l);
                }
            }
            a = a->next;
        }
}

void Parser::Error_Code4(){
    Parser::poly_dec *p =thePolyDeclarations;
    Parser::param_ID *ID;
    Parser::stmt * s = theSMTS;
    vector<int> *lineNu = new vector<int>;
    map<string,int> *temp = new map<string,int>;
    int count = 0;

    while (p!=NULL) // theDecs
    {
        string name = p->name;
        ID = p->theparam_IDs;
        while (ID!=NULL)
        {
            count++;
            ID=ID->next;
        }
        temp->insert({name,count});count = 0;
        p=p->next_poly_dec;
    }
    while (s!=NULL)
    {
        if (s->stmt_type == 1)
        {
            Parser::arg *a = s->p->theArgs;
            Token t = s->p->theToken;
            parse_EC4args(a,lineNu,temp,t);
        }
        s= s->next;
    }
    

     if (lineNu->size()>0)
     {
        sort(lineNu->begin(),lineNu->end());
        cout<< "Error Code 4: ";
        for (auto elem : *lineNu)
        {
            cout << elem << " ";
        }
     }
}
void Parser::parse_EC4args(Parser::arg *a ,vector<int> *l,map<string,int> *temp,Token t){
    int count=0;
    int expArgs = (*temp)[t.lexeme];
    while (a!= NULL)
    {
        if (a->arg_type==1)
        {
            Parser::arg *b = a->p->theArgs;
            Token t2 = a->p->theToken;
            parse_EC4args(b,l,temp,t2);
        }
        
        count++;
        a = a->next;
    }
    if (count!=expArgs)
    {
        l->push_back(t.line_no);
    }
}

Parser::poly_dec *Parser::parse_poly_decl_section(){
    Parser::poly_dec *head_poly_dec;
    head_poly_dec = parse_poly_decl();
    Token t = lexer.peek(1); 
    if (t.token_type==POLY)
    {
        head_poly_dec->next_poly_dec= parse_poly_decl_section();
    }
    return head_poly_dec;
}
 Parser::poly_dec *Parser::parse_poly_decl(){
    Parser::poly_dec *apoly_dec = new Parser::poly_dec;
    Token t = expect(POLY); 
    t =lexer.peek(1);
    apoly_dec->name = t.lexeme; apoly_dec->theToken = t;
    apoly_dec->theparam_IDs = parse_poly_header();
    expect(EQUAL); 
    apoly_dec->terms = parse_poly_body(); currentParams = NULL;
    expect(SEMICOLON); 
    return apoly_dec;
}
 
void Parser::parse_START(){
    expect(START);
    theSMTS =  parse_statement_list();
}
Parser::param_ID* Parser::parse_poly_header(){ // F(args)
    Parser::param_ID * theparam_IDs;
    parse_poly_name();
    Token t = lexer.peek(1);
    if (t.token_type == LPAREN)
    {
        expect(LPAREN);
        theparam_IDs = parse_id_list(0);
        expect(RPAREN);
    }else if (t.token_type==EQUAL)
    {
        theparam_IDs = new Parser::param_ID;
        theparam_IDs->ID = "x";
        theparam_IDs->order=0;
        theparam_IDs->next= NULL;
        currentParams = theparam_IDs;
        return theparam_IDs;
    }
    else
    {
        syntax_error();
    }
    currentParams = theparam_IDs;
    return theparam_IDs;
}
Parser::param_ID* Parser::parse_id_list(int n){
    Parser::param_ID * aparam_ID = new Parser::param_ID;
    Token t = expect(ID);
    aparam_ID->ID = t.lexeme;
    aparam_ID->order = n;
    t = lexer.peek(1);
    if (t.token_type == COMMA)
    {
        expect(COMMA);
        aparam_ID->next = parse_id_list(++n);
    }
    else if (t.token_type == RPAREN)
    {
        return aparam_ID;
    }
    else
    {
        syntax_error();
    }
    return aparam_ID;
}
Parser::term * Parser::parse_poly_body(){ // just calls term list  down there.
    return parse_term_list();
}

Parser::term * Parser::parse_term_list(){ // parses {3x^2 +2x + 2 } where each monomials and +- are terms
    Parser::term *head_term;
    head_term = parse_term();
    Token t = lexer.peek(1);
    if (t.token_type  == PLUS ||t.token_type  == MINUS)
    {
        head_term->addOperator =  parse_add_operator();
        head_term->next = parse_term_list();
    }
    else if (t.token_type == SEMICOLON)
    {
        head_term->addOperator = SEMICOLON; // to find end
    }
    else
    {
        syntax_error();
    }
    return head_term;
}
Parser::term * Parser::parse_term(){ // Term will either be x ,  3x, 3x^2, x^2y
    Parser::term *a_term = new Parser::term;
    Token t = lexer.peek(1);
    switch (t.token_type)
    {
    case ID:
        a_term->coefficient = 1;
        a_term->head_monomial = parse_monomial_list();
        break;
    case NUM:
        a_term->coefficient = parse_coefficient();
        t = lexer.peek(1);
        if (t.token_type == ID)
        {
            a_term->head_monomial = parse_monomial_list();
        }
        else
        {
            a_term->head_monomial = NULL;
        }
        break;
    default:
        syntax_error();
    }
    return a_term;
}
Parser::monomial*  Parser::parse_monomial_list(){ // 3xyz the monomial list is xyz
    Parser::monomial *head_monomial; 
    head_monomial = parse_monomial();
    Token t = lexer.peek(1);
    if (t.token_type==ID)
    {
        head_monomial->next = parse_monomial_list();
    }
    return head_monomial;   
}
Parser::monomial* Parser::parse_monomial(){ // 3x^2
    Parser::monomial *a_monomial = new Parser::monomial;
    
    Token t = expect(ID);
    a_monomial->theToken = t;
    
    // Parser::param_ID *p = currentParams;
    // while (p->ID!=t.lexeme) ORDERTEST
    // {
    //     p = p->next;
    // }
    // a_monomial->order = p->order;
    
    t = lexer.peek(1);
    if (t.token_type==POWER){
        a_monomial->exponent = parse_exponent();
    }
    else
    {
        a_monomial->exponent = 1;    
    }
    return a_monomial;
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
TokenType Parser::parse_add_operator(){
    Token t = lexer.peek(1);
    if (t.token_type==PLUS)
    {
        t = expect(PLUS);
    }else if (t.token_type==MINUS)
    {
        t = expect(MINUS);
    }else
    {
        syntax_error();
    }
    return t.token_type;
}
int Parser::parse_coefficient(){
    Token t = expect(NUM);
    return stoi(t.lexeme);
}
Parser::stmt *Parser::parse_statement_list(){
    Parser::stmt *head_stmt;
    head_stmt = parse_statement(); head_stmt->next = NULL;
    Token t = lexer.peek(1);
    if (t.token_type == ID || t.token_type == INPUT )
    {
        head_stmt->next = parse_statement_list();
    }
    return head_stmt;
}
Parser::stmt *Parser::parse_statement(){
    Parser::stmt *astmt = new Parser::stmt;
    Token t = lexer.peek(1);
    switch (t.token_type)
    {
    case INPUT:
        astmt->stmt_type = 0;
        astmt->poly_name = parse_input_statement().lexeme;
        // astmt->variable = variables[astmt->poly_name]; ORDERTEST
        break;
    case ID:
        astmt->stmt_type = 1;
        astmt->p = parse_poly_eval_statement(); 
        astmt->theToken = t;
        break;
    default:
        syntax_error();
    }
    return astmt;

}
Parser::poly_eval * Parser::parse_poly_eval_statement(){ // F(args);
    Parser::poly_eval *p;
    p = parse_poly_eval();
    expect(SEMICOLON);
    return p;
}
Token Parser::parse_input_statement(){
    expect(INPUT);
    Token t = expect(ID);
    allocateMem(t);
    expect(SEMICOLON);
    return t;
}
Parser::poly_eval * Parser::parse_poly_eval(){  // F(args)
    Parser::poly_eval * apoly_eval = new Parser::poly_eval;
    Token t = parse_poly_name();
    apoly_eval->theToken =t;
    apoly_eval->poly_name = t.lexeme;
    expect(LPAREN);
    apoly_eval->theArgs = parse_arg_list();
    expect(RPAREN);
    return apoly_eval;
}
Token Parser::parse_poly_name(){ // just gets ID of Poly, for instance Poly F, just gets F
    Token t = expect(ID);
    return t;
}
Parser::arg *Parser::parse_arg_list(){ // (x,y) x and y are the args
    Parser::arg *head_arg;
    head_arg = parse_arg();
    Token t = lexer.peek(1);
    if (t.token_type==COMMA)
    {
        expect(COMMA);
        head_arg->next =parse_arg_list();
    }
    return head_arg;
}
Parser::arg *Parser::parse_arg(){ // x, y inside parenthesis
    Parser::arg *anArg = new Parser::arg;
    Token t = lexer.peek(1);
    Token p = lexer.peek(2);
    switch (t.token_type)
    {
    case ID:
        if (p.token_type==LPAREN) 
        {
            anArg->theToken = t;
            anArg->arg_type = POLY;
            anArg->p = parse_poly_eval(); 
        }else
        {
            anArg->theToken = t;
            t = expect(ID);
            anArg->arg_type = t.token_type;
            // anArg->index = variables[t.lexeme]; // ORDERTEST
        }
        break;
    case NUM:
        t = expect(NUM);
        anArg->theToken = t;
        anArg->arg_type = t.token_type;
        anArg->value = stoi (t.lexeme);
        break;
    default:
        anArg->theToken = t;
        anArg->arg_type = POLY;
        anArg->p = parse_poly_eval(); 
        break;
    }
    return anArg;
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
    parser.parse_input();
    
	
}

