import random
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

from tqdm import tqdm
from sklearn.preprocessing import normalize
from sklearn.linear_model import RidgeClassifier
from sklearn.model_selection import train_test_split

def load_data(path):
    """
    Load data from a csv file, and return the feature vectors and the ground-truth labels.
    """

    df = pd.read_csv(path, encoding="utf-8")

    # Print the dataframe to see the data structure
    print(f"Loading Dataframe:\n{df}\n")

    # "black" is the true label, and the rest are feature vectors
    y = df["black"].to_numpy(dtype=np.int32)
    X = df.drop(columns=["black"]).to_numpy(dtype=np.float64)

    # Normalize the feature vectors to avoid overflow problem
    X = normalize(X, axis=0, norm="max")

    return X, y


def kfold_cross_val_score(model, X, y, k=10):
    """
    Evaluate a score by cross-validation
    :param model: object that implements 'fit' and 'predict'
        An estimator object
    :param X: array-like, shape (n_samples, n_features)
        The data to fit.
    :param y: array-like, shape (n_samples,)
        The target variable to try to predict. Must be binay.
    :param k: int, default=5
        Number of folds
    :return: array, shape (k,)
        Array of accuracy scores of the estimator for each run of the cross validation.
    """

    N = X.shape[0]
    fold_size = N // k
    scores = []

    for i in tqdm(range(k), desc="Cross-validation"):
        # TODO: (Problem 3.1) Implement the cross-validation procedure.
        # You should split the dataset into training and validation set, then train
        # and evaluate the accuracy of the model using the training and validation set.
        # ==== START OF YOUR CODE (DO NOT DELETE THIS LINE) ====

        # Split the dataset into training and test set
        X_train = np.concatenate([X[: i * fold_size], X[(i + 1) * fold_size :]], axis=0)
        y_train = np.concatenate([y[: i * fold_size], y[(i + 1) * fold_size :]], axis=0)
        X_val = X[i * fold_size : (i + 1) * fold_size]
        y_val = y[i * fold_size : (i + 1) * fold_size]

        # Train the model
        model.fit(X_train, y_train)

        # Evaluate the model
        y_pred = model.predict(X_val)  # y_pred is the predicted label
        accuracy = np.mean(y_pred == y_val)

        # or, you can use the following code to evaluate the model
        # accuracy = model.score(X_val, y_val)

        # ==== END OF YOUR CODE (DO NOT DELETE THIS LINE) ====
        scores.append(accuracy)

    return np.array(scores)

def plot_cv_scores(alpha_scores):
    """
    Plot the accuracy distribution for each alpha value in cross-validation
    """

    fig = plt.figure(dpi=200)

    for alpha in alpha_scores:
        scores = alpha_scores[alpha]
        plt.scatter([alpha] * len(scores), scores)
        plt.errorbar(alpha, np.mean(scores), yerr=np.std(scores), fmt="o")

    # draw the mean accuracy
    mean_scores = np.array([np.mean(alpha_scores[alpha]) for alpha in alpha_scores])
    plt.plot(alpha_list, mean_scores, label="Mean", color="blue")

    plt.title("Accuracy vs. alpha in Ridge Regression")
    plt.xscale("log")
    plt.xlabel("alpha")
    plt.ylabel("Accuracy")
    
    fig.savefig("cross_validation.png")
    print("\nPlot is saved as cross_validation.png\n")



if __name__ == "__main__":
    # Fix the random seed for reproducibility
    random.seed(42)
    np.random.seed(42)

    DATASET_PATH = "data.csv"
    K_FOLD = 10

    # Load dataset, and print the shape of the feature vectors and the true labels
    X, y = load_data(DATASET_PATH)
    print("Shape of X:", X.shape)
    print("Shape of y:", y.shape)

    # Split the 80% of the dataset as the training set, and the rest as the test set
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
    print("Shape of X_train:", X_train.shape)
    print("Shape of y_train:", y_train.shape)
    print("Shape of X_test:", X_test.shape)
    print("Shape of y_test:", y_test.shape)

    # Generate 8 candidate alpha values in the range of 10^-4 to 10^4
    alpha_list = np.logspace(-4, 4, 8)
    print(f"\nCandidate alpha values: {alpha_list}\n")
    alpha_scores = {}

    for alpha in alpha_list:
        # TODO: (Problem 3.2) For each alpha value, initialize a RidgeClassifier with that alpha value,
        # and evaluate the model using 10-fold cross-validation
        # Hint: use `RidgeClassifier` from sklearn, and set the `alpha` parameter to the given alpha value
        # ==== START OF YOUR CODE (DO NOT DELETE THIS LINE) ====
        
        model = RidgeClassifier(alpha=alpha)
        scores = kfold_cross_val_score(model, X_train, y_train, k=K_FOLD)

        # ==== END OF YOUR CODE (DO NOT DELETE THIS LINE) ====
        score = np.mean(scores)
        alpha_scores[alpha] = scores
        print(f"alpha = {alpha:10.4f}, average accuracy = {score:.4f}")

    # Plot the average accuracy for each C value
    plot_cv_scores(alpha_scores)

    # TODO: (Problem 3.3) Find the best alpha value, train a RidgeClassifier with it, then evaluate the model using test set
    # ==== START OF YOUR CODE (DO NOT DELETE THIS LINE) ====

    best_alpha = max(alpha_scores, key=lambda x: np.mean(alpha_scores[x]))
    best_model = RidgeClassifier(alpha=best_alpha)
    best_model.fit(X_train, y_train)
    y_pred = best_model.predict(X_test)
    accuracy = np.mean(y_pred == y_test)

    # ==== END OF YOUR CODE (DO NOT DELETE THIS LINE) ====

    print(f"Best alpha = {best_alpha:.4f}, accuracy on test set = {accuracy:.4f}")
