with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with ada.strings.unbounded; use ada.strings.unbounded;
with ada.strings.unbounded.Text_IO; use ada.strings.unbounded.Text_IO;
with ada.characters.latin_1; use ada.characters.latin_1;
with Ada.Characters.Handling; use Ada.Characters.Handling;
with Ada.Strings.Fixed; use Ada.Strings.Fixed;
with Ada.Strings.Maps;
with ada.Float_Text_IO; use ada.Float_Text_IO;
procedure Textyzer is

    s : unbounded_string;
    infp : file_type;
    len_1 : integer:= 0;
    a: array (1..20) of integer := (1..20 => 0);
    flag : Boolean;
 
    function getFilename return string is     -- Getting the filename here
        fileName : unbounded_string;
        value : integer;
    begin
        loop
        begin
        value := -1;
        put("Enter FileName:");
        get_line(fileName);
        open(infp,In_File, to_string(fileName));
        close(infp);
        exception
            when Name_Error =>          -- checking if the given name is correct or not
                value := 1;
        end;
        exit when value = -1;
        end loop;
        return to_string(fileName);    
    end getFilename;



    function isWord(word: string) return Boolean is     
        theWord : unbounded_string;
        isWordFlag : Boolean;
        wordLen : integer;
       -- value : integer;
    begin
       wordLen := length(to_unbounded_string(word));
    
    for i in 1..20 loop
        if wordLen = i then     -- Filling the array which houses the histogram
            len_1 := len_1+1;       
            a(i) := a(i) + 1; 
        end if;
    end loop;

        isWordFlag := true;
        for i in word'First..word'Last loop
            if is_letter(word(i)) /= true then -- Checking if the word is actually a word
                isWordFlag := false;
            end if;
        end loop;

        --put(word);
       -- put(" ");
      
        return isWordFlag;   
    end isWord;



    function isNumber(word: string) return Boolean is     
        theWord : unbounded_string;
        isWordFlag : Boolean;
      
    begin
        isWordFlag := true;
        for i in word'First..word'Last loop
            if is_digit(word(i)) /= true then  -- Checking for the number purity
                isWordFlag := false;
            end if;
        end loop;
        
        return isWordFlag;    
    end isNumber;



    function analyzeText(fileName: string) return string is     
        openName : unbounded_string;
        lineCount : integer;
        totLen : integer;
        totSents : integer;
        totWords : integer;
        totNums : integer;
        totPunc : integer;
        buffer : unbounded_string;  
        first : integer;
        i : integer;
        finish: integer;
        Output : unbounded_string;
        avglet: float := 0.0;
    begin
        
        lineCount := 0;
        totSents := 0;
        first := 1;
        finish:= 0;
        totWords := -1;
        totPunc := 0;

        open(infp,in_file,fileName);
        loop
        exit when end_of_file(infp);
        get_line(infp,s);
        buffer := buffer & s & LF;      -- Filling my buffer containing all of the data
        lineCount := lineCount + 1;
        totLen := length(s);
        i := 0;
        first := 1;
        totNums:= -1;
           
            for i in 1..totLen-1 loop -- Calling isWOrd on a per line basis

                if to_string(s)(i) = ' ' or to_string(s)(i) = ',' or to_string(s)(i) = '.' or to_string(s)(i) = '!' or to_string(s)(i) = '?' or to_string(s)(i) = LF then
                    finish:= i;
                    if isWord(to_string(s)(first..finish-1)) then 
                        totWords := totWords +1;
                    end if;
                    first:= finish+1;
                end if;

            end loop;

            for i in 1..totLen-1 loop -- Callling is num on a per line basis

                if to_string(s)(i) = ' ' or to_string(s)(i) = ',' or to_string(s)(i) = '.' then
                        finish:= i;
                        if isNumber(to_string(s)(first..finish-1)) then 
                            totNums := totNums +1;
                        end if;
                        first:= finish+1;
                end if;

            end loop;

        end loop;
        close(infp);
        totLen := length(buffer);
        
        i := 0;
        for i in 1..totLen loop

            if to_string(buffer)(i) = '.' then -- Calculating all other metrics
                totSents := totSents + 1;
            end if;
            if to_string(buffer)(i) = ',' or to_string(buffer)(i) = ';' or to_string(buffer)(i) = '.' or to_string(buffer)(i) = '?' or to_string(buffer)(i) = '!' then
                totPunc := totPunc + 1;
            end if;
            

        end loop;
        

        new_line;
            -- FILLING UP DESIRED RESULT
        buffer := to_unbounded_string("number of characters: ") & to_unbounded_string(Integer'Image (totLen)) & LF;
        buffer := buffer & to_unbounded_string("number of words: ") & to_unbounded_string(Integer'Image (totWords)) & LF;
        buffer := buffer & to_unbounded_string("number of numbers: ") & to_unbounded_string(Integer'Image (totNums)) & LF;
        buffer := buffer & to_unbounded_string("number of sentences: ") & to_unbounded_string(Integer'Image (totSents)) & LF;
        buffer := buffer & to_unbounded_string("number of lines: ") & to_unbounded_string(Integer'Image (lineCount)) & LF;
        buffer := buffer & to_unbounded_string("number of punctuation: ") & to_unbounded_string(Integer'Image (totPunc)) & LF;
        buffer := buffer & to_unbounded_string("Average letters per word: ") & to_unbounded_string(Float'Image ((Float(totLen)/Float(totWords)))) & LF;
        buffer := buffer & to_unbounded_string("Average letters per sentence: ") & to_unbounded_string(Float'Image ((Float(totWords)/Float(totSents)))) & LF;

              
      --  put(buffer);
        
        return to_string(buffer);    

    end analyzeText;

    function printHist return Boolean is

    begin
    for i in 1..20 loop
        new_line;
        put(i,2);
        put("   ");
        for j in 1..a(i) loop   -- Printing the histogram
            put("*");
        end loop;
    end loop;

        return true;
    end printHist;


begin
    --program to print a greeting message
    

    new_line;
   put(to_unbounded_string(analyzeText(getFilename))); -- printing everything
    flag := printHist;
   

end Textyzer;