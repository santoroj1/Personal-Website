//***************************************************************
//
// Program Name:  GCContentCalculator.java
//
// Programmers:   John Santoro and Joseph Young
//
// Description:   Counts the total number of A's, T's, G's, and 
//                C's in a strand of DNA and then computes the 
//                %G~C content of that particular strand.
//
//***************************************************************

import java.util.Scanner;

public class GCContentCalculator
{
   //---------------------------------------------------------------
   // The main method accepts user input for the DNA strand.  Using
   // that input, it calculates the number of A's, T's, G's, and C's
   // and then determines whether a wrong character was entered.
   // Afterwards, it finds the %G~C in the strand and then prints 
   // the results.
   //---------------------------------------------------------------
   public static void main(String[] args)
   {
      String dnaSequence;
      char ch;
      int countA, countT, countG, countC;
      boolean wrongCharacterExists = false;
      int totalATGC;
      double percentGCToTotal;
      
      Scanner scan = new Scanner(System.in);
      
      System.out.println("Enter a DNA sequence:  ");
      
      // Gets a DNA sequence
      dnaSequence = scan.nextLine();
      
      // The length of the string
      int stringLength = dnaSequence.length();
      
      // Initializes counts
      countA = 0;
      countT = 0;
      countG = 0;
      countC = 0;
      
      // Total count of all bases
      for (int i = 0; i < stringLength; i++)
      {
         ch = dnaSequence.charAt(i);
         
         switch (ch)
         {
            case 'A' :
            case 'a' :
               countA++;
               break;
               
            case 'T' :
            case 't' :
               countT++;
               break;
               
            case 'G' :
            case 'g' :
               countG++;
               break;
               
            case 'C' :
            case 'c' :
               countC++;
               break;
            
            // Determines whether a wrong character exists
            default :
               wrongCharacterExists = true;
         }
      }
      // The total of all A's, G's, C's, and T's
      totalATGC = countC + countG + countT + countA;
         
      // Calculates the percentage of g and c in the strand
      percentGCToTotal = (countG + countC) / (double)(totalATGC) * 100;
      
      // Prints out the results
      if (wrongCharacterExists)
         System.out.println("Incorrect characters found in sequence");
      
      System.out.println("Number of A's:  " + countA);
      System.out.println("Number of T's:  " + countT);
      System.out.println("Number of G's:  " + countG);
      System.out.println("Number of C's:  " + countC);
      
      System.out.println("Total number of A's, G's, T's, and C's:  " + totalATGC);
      
      System.out.println("%G~C Content:  " + percentGCToTotal);
   }
}