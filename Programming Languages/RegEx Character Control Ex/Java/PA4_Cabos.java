// Alexander Cabos
// PA4 - pa4_Cabos.py
// 3/22/23


package PA4.PA4_Cabos;


import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.regex.*;

public class PA4_Cabos {
    public static void main(String[] args) {
        try {
            // Read the test data from the file
            byte[] data = Files.readAllBytes(Paths.get("control-char.txt"));

            // Fixes all the non-printable characters
            String fixed_data = new String(data, StandardCharsets.UTF_8)
                    .replace("\u0005\r\n", "").replace("\u0006\r\n ", "")
                    .replace("\r\n kf \u0017\u0012'\r\n jf", "")
                    .replace("\u0003", "").replace(" \u0002", "");

            // Remove extra spaces between words
            fixed_data = Pattern.compile("[^\\n\\S]+").matcher(fixed_data).replaceAll(" ");

            // Remove extra letters at the end of a line
            fixed_data = Pattern.compile("\\s[A-Z]\\s\\n").matcher(fixed_data).replaceAll("\n");

            // Fix space in the middle of "line" on line 3, I couldn't figure out another way to get this done.
            fixed_data = fixed_data.replaceAll("i\\s[n]", "in");

            // Update file with corrected data
            BufferedWriter writer = new BufferedWriter(new FileWriter("control-char.txt"));
            writer.write(fixed_data);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
