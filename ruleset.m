ClassDefinition ::= "class_name" Identifier
Inheritance ::= "extends" Identifier
FunctionDeclaration ::= "func" Identifier + "(" Parameter[] ")" "->" Return ":" Block "pass"
VariableAssignment ::= "var" Identifier ":" Type "=" Expression;
Expression ::= Literal | Identifier | BinaryOp | FunctionCall | ...
