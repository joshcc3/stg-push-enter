module Parser where

import Text.Parsec.Language 
import Text.Parsec.Token as TP
import Types
import Control.Applicative

stgDef :: LanguageDef st
stgDef = haskellStyle {
    reservedOpNames =  ["|", "=", "->", "+#", "-#", "==#", ">#", "<#"],
    reservedNames = ["data", "let", "in", "case", "of"]
}
stg = makeTokenParser stgDef
program = Program <$> many1 conDecl <*> many1 funDef
funDef = (,) <$> (whiteSpace *> TP.identifier) <*> object
conDecl = ConDecl <$> 
    (whiteSpace *> string "data" *> TP.identifier <* string "=" <* whiteSpace) <*>
    (many1 conDefn <* string "|" <* whiteSpace)
  where
    conDefn = (,) <$> TP.identifier <*> many1 field
    field = fmap f TP.identifier 
    f x = if take (length "Boxed") x == "Boxed" then (x, Boxed) else (x, Unboxed)

object = FUNC <$> function  <|> CON <$> constructor <|>  PAP <$> pap <|> THUNK <$> thunk 
constructor = Con <$> TP.identifier <*> many1 TP.identifier
pap = PAP <$> (TP.identifier <* string "$!" <* whiteSpace) <*> many1 atom
thunk = expr
function = Fun <$> (string "\\" *> whiteSpace *> many1 arg) <*> expr
  where
    arg = TP.parens ((,) <$> (TP.identifier <* whiteSpace <* string "," <* whiteSpace) <*> TP.identifier)

lit = (L . fromInteger) <$> TP.integer
var = V <$> TP.identifier
primop = liftA2 P TP.operator . liftA2 (:) atom $ fmap (:[]) atom 
atom = lit <|> var <|> primop

expr = (Atom <$> atom) <|> letExpr <|> caseExpr <|> funcExpr
letExpr = Let <$> 
  (whiteSpace *> string "let" *> whiteSpace *> TP.identifier <* whiteSpace) <*> 
  (object <* whiteSpace) <*> 
  (expr <* whiteSpace)
caseExpr = AltCase <$> 
    (whiteSpace *> string "case" whiteSpace) <*> 
    (atom <* whiteSpace <* string "of" <* whiteSpace) <*> 
    alts

alts = many1 alt
  where
    alt = Alt <$> TP.identifier <*> (many1 TP.identifier) <*> expr





main = let one = I# 1
       let inc = THUNK (plus_int one)
       let inced = THUNK (map inc list)
       let list = Cons one inced
       let element1 = THUNK (head inced)
       let tail1 = THUNK (tail inced)
       let element2 = THUNK (head tail1)
       in case plus_int element1 element2 of
          I# z -> print_int z

(identifier <* whiteSpace) 

expression = 