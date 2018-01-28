using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace JohnSantoroFinalProject
{
    public partial class Calculator : Form
    {
        /// <summary>
        /// Use list as stack
        /// </summary>
        List<double> entries;

        bool decimalPlaced;
        bool justPerformedOp;
        bool errorExists;
        bool justPressedEquals;

        /// <summary>
        /// Stores internal memory of calculator.  When count == 0, the memory is empty.
        /// </summary>
        List<double> memory;

        /// <summary>
        /// Initialize component and default string.
        /// </summary>
        public Calculator()
        {
            InitializeComponent();

            entries = new List<double>();
            entries.Add(0);

            CalculatorOutBox.Text = entries[0].ToString() + ".";
            decimalPlaced = false;
            justPerformedOp = false;
            errorExists = false;
            justPressedEquals = false;

            memory = new List<double>();
        }

        #region Helper Methods
        void PerformOperation(double Opcode)
        {
            double trouble = 0;

            if (Opcode == 12)  // Division
            {
                trouble = Convert.ToDouble(entries[2] / entries[0]);

                if (!double.IsInfinity(trouble) && !double.IsNaN(trouble))
                {
                    CalculatorOutBox.Text = Convert.ToString(trouble) + (trouble % 1 == 0 ? "." : "");
                    entries[0] = trouble;
                }
                else
                {
                    CalculatorOutBox.Text = "DIV / 0 ERROR";
                    errorExists = true;
                }
            }
            else if (Opcode == 13)  // Multiplication
            {
                trouble = Convert.ToDouble(entries[2] * entries[0]);
                CalculatorOutBox.Text = Convert.ToString(trouble) + (trouble % 1 == 0 ? "." : "");
                entries[0] = trouble;
            }
            else if (Opcode == 14)  // Subtraction
            {
                trouble = Convert.ToDouble(entries[2] - entries[0]);
                CalculatorOutBox.Text = Convert.ToString(trouble) + (trouble % 1 == 0 ? "." : "");
                entries[0] = trouble;
            }
            else if (Opcode == 15)  // Addition
            {
                trouble = Convert.ToDouble(entries[2] + entries[0]);
                CalculatorOutBox.Text = Convert.ToString(trouble) + (trouble % 1 == 0 ? "." : "");
                entries[0] = trouble;
            }
        }

        void AlterEntry(int Opcode)
        {
            if (!errorExists)  // Do not alter entry if it contains an error.
            {
                if (Opcode <= 9)  // Number entries
                {
                    if (!decimalPlaced)
                    {
                        if (CalculatorOutBox.Text == "0." || justPerformedOp)
                        {
                            CalculatorOutBox.Text = Opcode.ToString() + ".";
                            justPerformedOp = false;
                            justPressedEquals = false;
                        }
                        else
                        {
                            CalculatorOutBox.Text = CalculatorOutBox.Text.TrimEnd('.') +
                                Opcode.ToString() + ".";
                        }
                    }
                    else
                        CalculatorOutBox.Text += Opcode.ToString();
                }
                else if (Opcode <= 11)  // Additional Entry Options
                {
                    if (justPerformedOp)  // Reset if answer was just computed and it was not due to the solve button.
                    {
                        if (!justPressedEquals)
                        {
                            justPerformedOp = false;
                            CalculatorOutBox.Text = "0.";
                        }
                        else  // Otherwise, do not clear.
                        {
                            justPressedEquals = false;
                        }
                    }

                    if (Opcode == 10)
                    {
                        if (CalculatorOutBox.Text[0] == '-')
                            CalculatorOutBox.Text = CalculatorOutBox.Text.TrimStart('-');
                        else if (CalculatorOutBox.Text != "0.")
                            CalculatorOutBox.Text = "-" + CalculatorOutBox.Text;
                    }
                    else if (Opcode == 11)
                    {
                        decimalPlaced = true;
                    }
                }
                else if (Opcode <= 15)  // Arithmetic Operations
                {
                    if (!justPerformedOp)  // Do not allow repeated calculations if an operation has just been performed and the output hasn't been replaced.
                    {
                        if (entries.Count == 1)
                        {
                            entries[0] = Convert.ToDouble(CalculatorOutBox.Text);
                            // CalculatorOutBox.Text = "0.";
                            decimalPlaced = false;
                            justPerformedOp = true;
                            entries.Add(Opcode);
                            entries.Add(0);
                        }
                        else if (entries.Count == 3)
                        {
                            entries[2] = entries[0];
                            entries[0] = Convert.ToDouble(CalculatorOutBox.Text);

                            justPerformedOp = true;
                            PerformOperation(entries[1]);

                            entries[1] = Opcode;
                            decimalPlaced = false;
                        }
                    }
                    else  // However, do allow for replacement of operations.
                    {
                        if (Opcode >= 12 && Opcode <= 15)
                            entries[1] = Opcode;

                        if (justPressedEquals)
                        {
                            entries.Clear();
                            entries.Add(0);

                            entries[0] = Convert.ToDouble(CalculatorOutBox.Text);
                            // CalculatorOutBox.Text = "0.";
                            decimalPlaced = false;
                            justPerformedOp = true;
                            entries.Add(Opcode);
                            entries.Add(0);
                        }
                    }
                }
                else if (Opcode == 16)  // Solve button
                {
                    if (entries.Count == 3)
                    {
                        if (!justPerformedOp)
                        {
                            entries[2] = entries[0];
                            entries[0] = Convert.ToDouble(CalculatorOutBox.Text);
                        }

                        double temp = entries[0];

                        justPerformedOp = true;
                        PerformOperation(entries[1]);

                        entries[2] = entries[0];
                        entries[0] = temp;  // Replacement value due to function

                        decimalPlaced = false;
                        justPerformedOp = true;
                        justPressedEquals = true;
                    }
                }
                else if (Opcode <= 19)  // Special Ops -- Simply solve whatever is in the text box and clear previous entries.
                {
                    if (Opcode == 17)  // Square root
                    {
                        // Convert current entry to square root.
                        double trouble = Convert.ToDouble(CalculatorOutBox.Text);

                        if (trouble >= 0)
                        {
                            trouble = Math.Sqrt(trouble);

                            if (trouble % 1 == 0)
                            {
                                CalculatorOutBox.Text = trouble.ToString() + ".";
                                decimalPlaced = false;
                            }
                            else
                            {
                                CalculatorOutBox.Text = trouble.ToString();
                                decimalPlaced = true;
                            }
                        }
                        else  // Show error if attempt is made at getting square root of a negative number.
                        {
                            CalculatorOutBox.Text = "SQRT(x < 0) ERRROR";
                            errorExists = true;
                        }
                    }
                    else if (Opcode == 18)  // Percent button --> Take entry and compute [n]% of that entry
                    {
                        double trouble = Convert.ToDouble(CalculatorOutBox.Text);

                        if (entries.Count == 1)  // If there is only 1 entry, this computation will not work.
                        {
                            CalculatorOutBox.Text = "0.";
                            decimalPlaced = false;
                        }
                        else if (entries.Count == 3)
                        {
                            trouble = entries[0] * trouble / 100.0;
                            
                            if (trouble % 1 == 0)
                            {
                                CalculatorOutBox.Text = trouble.ToString() + ".";
                                decimalPlaced = false;
                            }
                            else
                            {
                                CalculatorOutBox.Text = trouble.ToString();
                                decimalPlaced = true;
                            }
                        }
                    }
                    else if (Opcode == 19)  // 1/x button --> Take entry and compute 1/n of that entry
                    {
                        // Invert entry
                        double trouble = Convert.ToDouble(CalculatorOutBox.Text);

                        if (Math.Abs(trouble) >= 0)
                        {
                            trouble = 1 / trouble;

                            if (trouble % 1 == 0)
                            {
                                CalculatorOutBox.Text = trouble.ToString() + ".";
                                decimalPlaced = false;
                            }
                            else
                            {
                                CalculatorOutBox.Text = trouble.ToString();
                                decimalPlaced = true;
                            }
                        }
                        else  // Show error if attempt is made at getting the inverse of 0.
                        {
                            CalculatorOutBox.Text = "DIV / 0 ERRROR";
                            errorExists = true;
                        }
                    }
                }
            }
        }
        #endregion

        #region Digits
        private void Button9Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(9);
        }

        private void Button8Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(8);
        }

        private void Button7Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(7);
        }

        private void Button6Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(6);
        }

        private void Button5Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(5);
        }

        private void Button4Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(4);
        }

        private void Button3Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(3);
        }

        private void Button2Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(2);
        }

        private void Button1Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(1);
        }

        private void Button0Digit_Click(object sender, EventArgs e)
        {
            AlterEntry(0);
        }
        #endregion

        #region Additional Entry Buttons
        private void SignButton_Click(object sender, EventArgs e)
        {
            AlterEntry(10);
        }

        private void DecimalButton_Click(object sender, EventArgs e)
        {
            AlterEntry(11);
        }
        #endregion

        #region Operation Buttons
        private void DivisionButton_Click(object sender, EventArgs e)
        {
            AlterEntry(12);
        }

        private void MultiplyButton_Click(object sender, EventArgs e)
        {
            AlterEntry(13);
        }

        private void SubtractionButton_Click(object sender, EventArgs e)
        {
            AlterEntry(14);
        }

        private void AdditionButton_Click(object sender, EventArgs e)
        {
            AlterEntry(15);
        }

        private void SqrtButton_Click(object sender, EventArgs e)
        {
            AlterEntry(17);
        }

        private void PercentButton_Click(object sender, EventArgs e)
        {
            AlterEntry(18);
        }

        private void InverseButton_Click(object sender, EventArgs e)
        {
            AlterEntry(19);
        }

        private void SolveButton_Click(object sender, EventArgs e)
        {
            AlterEntry(16);
        }
        #endregion

        #region Memory Buttons
        /// <summary>
        /// Clears the contents of the memory.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ClearMemoryButton_Click(object sender, EventArgs e)
        {
            memory.Clear();
            MemoryExistsBox.Text = "";
        }

        /// <summary>
        /// Gets memory and puts it into output box.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MemoryRecallButton_Click(object sender, EventArgs e)
        {
            if (memory.Count > 0)
            {
                if (memory[0] % 1 == 0)
                {
                    CalculatorOutBox.Text = memory[0].ToString() + ".";
                    decimalPlaced = false;
                }
                else
                {
                    CalculatorOutBox.Text = memory[0].ToString();
                    decimalPlaced = true;
                }
            }
        }

        /// <summary>
        /// Stores number in output box and puts it into memory
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MemoryStoreButton_Click(object sender, EventArgs e)
        {
            if (memory.Count == 0)
                memory.Add(Convert.ToDouble(CalculatorOutBox.Text));
            else
                memory[0] = Convert.ToDouble(CalculatorOutBox.Text);

            MemoryExistsBox.Text = "M";
        }

        /// <summary>
        /// Adds the number in the output box to the one currently in memory.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AddToMemoryButton_Click(object sender, EventArgs e)
        {
            if (memory.Count == 0)
                memory.Add(Convert.ToDouble(CalculatorOutBox.Text));
            else
                memory[0] += Convert.ToDouble(CalculatorOutBox.Text);

            MemoryExistsBox.Text = "M";
        }
        #endregion

        private void ClearButton_Click(object sender, EventArgs e)
        {
            entries.Clear();
            entries.Add(0);

            CalculatorOutBox.Text = entries[0] + ".";
            decimalPlaced = false;
            justPerformedOp = false;
            errorExists = false;
            justPressedEquals = false;
        }

        /// <summary>
        /// Deletes one character from the end of an input.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void BackspaceButton_Click(object sender, EventArgs e)
        {
            if (!justPerformedOp)  // Do not allow deletion if answer is what is being deleted.
            {
                if (CalculatorOutBox.Text.Length > 2)  // All entries other than 1 digit
                {
                    if (CalculatorOutBox.Text[0] != '-' || CalculatorOutBox.Text.Length > 3)
                    {
                        if (CalculatorOutBox.Text[CalculatorOutBox.Text.Length - 1] == '.')
                        {
                            CalculatorOutBox.Text = CalculatorOutBox.Text.Remove(CalculatorOutBox.Text.Length - 2, 1);
                            decimalPlaced = false;
                        }
                        else
                            CalculatorOutBox.Text = CalculatorOutBox.Text.Remove(CalculatorOutBox.Text.Length - 1);
                    }
                    else if (CalculatorOutBox.Text[0] == '-' && CalculatorOutBox.Text.Length <= 3)
                    {
                        CalculatorOutBox.Text = "0.";
                        decimalPlaced = false;
                    }
                }
                else
                {
                    CalculatorOutBox.Text = "0.";
                    decimalPlaced = false;
                }
            }
        }

        /// <summary>
        /// Clears entry, along with any error.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ClearEntryButton_Click(object sender, EventArgs e)
        {
            CalculatorOutBox.Text = "0.";
            decimalPlaced = false;

            if (errorExists)
            {
                entries.Clear();
                entries.Add(0);

                justPerformedOp = false;
                justPressedEquals = false;
                errorExists = false;
            }
        }
    }
}
