module Parser where

import Text.Parsec.Language 
import Text.Parsec.Token as TP
import Text.Parsec
import Text.Parsec.Combinator
import Types
import Control.Applicative(liftA2)
import Text.ParserCombinators.Parsec.Char


stgDef :: LanguageDef st
stgDef = haskellStyle {
    reservedOpNames =  ["|", "=", "->", ";"],
    reservedNames = ["data", "let", "in", "case", "of", "Boxed", "Unboxed"],
    identStart = lower
}

conDef :: LanguageDef st
conDef = stgDef {
  identStart = upper
}

primopDef :: LanguageDef st
primopDef = stgDef {
  identStart = char '#'
}

stg = makeTokenParser stgDef
con = makeTokenParser conDef
primops = makeTokenParser primopDef

program :: Parsec String () Program
program = Program <$> many1 conDecl <*> many1 funDef

funDef = (,) <$> (TP.identifier stg <* reservedOp stg "=") <*> object
conDecl = ConDecl <$> 
    (reserved stg "data" *> TP.identifier con <* reservedOp stg "=") <*>
    (zipWith toDefn [0..] <$> many1 (try (conDefn <* reservedOp stg "|" )))
  where
    toDefn ix (n, fs) = ConDefn n ix fs
    conDefn = (,) <$> TP.identifier con <*> try (many field)
    field = fmap f $ TP.identifier con
    f x = if x !! (length x - 1) == 'B' then (x, Unboxed) else (x, Boxed)
      where
        l = length x


object = FUNC <$> function  <|> CON <$> constructor <|> THUNK <$> thunk 
constructor = Con <$> TP.identifier con <*> many atom
pap = Pap <$> (TP.identifier stg <* reservedOp stg "$!" ) <*> many1 atom
thunk = expr
function = Fun <$> (reservedOp stg "\\" *> many1 arg <* reservedOp stg "->") <*> expr
  where
    arg = TP.parens stg ((,) <$> (TP.identifier stg) <*> typ)
    typ = (Boxed <$ reserved stg "Boxed") <|> (Unboxed <$ reserved stg "Unboxed")

lit = (L . fromInteger) <$> TP.integer stg
var = V <$> TP.identifier stg
primop = P <$> (parens stg  helper <|> helper)
    where
        helper = liftA2 Primop (TP.identifier primops) (many atom)
atom = try primop <|> lit <|> var


expr = try caseExpr <|> try letExpr <|> try funcCallExpr <|> (Atom <$> try atom)
letExpr = Let <$>
  (reserved stg "let" *> TP.identifier stg <* reservedOp stg "=") <*>
  (object) <*>
  (reserved stg "in" *> expr)

caseExpr = Case <$>  (reserved stg "case" *> atom <* reserved stg "of") <*> alts <* TP.reservedOp stg ";"
  where
    alts = many1 alt
    alt = AltCase <$> TP.identifier con <*> (many (TP.identifier stg)) <*>
        (reservedOp stg "->" *> expr)

funcCallExpr = FuncCall <$> TP.identifier stg <*> many1 atom


-- main = let one = I# 1
--        let inc = THUNK (plus_int one)
--        let inced = THUNK (map inc list)
--        let list = Cons one inced
--        let element1 = THUNK (head inced)
--        let tail1 = THUNK (tail inced)
--        let element2 = THUNK (head tail1)
--        in case plus_int element1 element2 of
--           I# z -> print_int z
--
-- (identifier <* whiteSpace)
--
-- expression =