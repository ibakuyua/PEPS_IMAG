#ifndef CCR_MODEL_TREE_MODEL_HPP
#define CCR_MODEL_TREE_MODEL_HPP

#define UP_DEFAULT 1.08
#define FREE_RISK_RATE 0.02

class Tree_model {
public:
    /**
     * Constructor (spot contain nb_index values)
     */
    Tree_model(double *maturity, int nb_step, int nb_index, double *spots, double *volatilities);

    /**
     * Get the price at start date of the multimonde contract
     */
    double Get_price();
    /**
     * Permit to display the tree (stock values)
     */
    void display_tree();

    ~Tree_model();
private:
    double *maturity; /*<each date for fixing performance : nb_index + 1 date with maturity[0] = start date = 0 */
    int nb_step; /*<number of step between each fixing dates*/
    int nb_index; /*<number of index (=size of maturity)*/
    double *stocks; /*<index values*/
    double *up_values; /*<up values step by step*/
    double *down_values; /*<down values step by step*/
    double *volatilities; /*<volatilities*/

    /**
     * Used to fill the tree index values
     */
    void Fill_tree(double *spots);
};


#endif //CCR_MODEL_TREE_MODEL_HPP
