\LoadClass{article}
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{LabReport}
              [2023/04/01 v0.1
 Kong's Report Template. Published on zhihu.com on 2023/04/01.]
% 中文支持
\RequirePackage[UTF8,heading = true]{ctex} 
% 页面布局和页边距
\RequirePackage[a4paper]{geometry}
% 字体管理
\RequirePackage{fontspec} 
\fontsize{12pt}{\baselineskip}\selectfont
% 数学支持
\RequirePackage{amssymb}
\RequirePackage{amsfonts}
\RequirePackage{amsmath}
\RequirePackage{esint}
\RequirePackage{gensymb}
\RequirePackage{mathtools}
\RequirePackage{amsthm} % 定理环境
% 超链接
\RequirePackage{hyperref}
% 页眉页脚
\RequirePackage{fancyhdr}

% 代码和伪代码支持

% 图片和表格
\usepackage{graphicx}
\usepackage{booktabs}
\usepackage{caption}
\captionsetup{font={small}}
\RequirePackage{diagbox} % 导入 diagbox 宏包，用于创建带有斜线的单元格
\RequirePackage{array} % 导入 array 宏包，用于调整列格式
% 绘图
\RequirePackage{pgfplots}
\pgfplotsset{compat=1.17}

% 参考文献管理
\RequirePackage[backend=biber,style=gb7714-2015,hyperref=true]{biblatex}
\renewcommand*{\bibfont}{\small}

% 标题格式
\RequirePackage{titlesec}
\renewcommand{\thesection}{\chinese{section}、}
\renewcommand{\thesubsection}{\arabic{section}.\arabic{subsection}}
% section 标题样式设置
\titleformat{\section}
{\normalfont\Large\bfseries\centering} % 格式
{\thesection} % 标签
{0em} % 间距
{} % 前置代码
% subsection 标题样式设置
\titleformat{\subsection}
{\normalfont\large\bfseries} % 格式
{\thesubsection} % 标签
{0em} % 间距
{} % 前置代码

% 列表优化
\RequirePackage{enumitem}
\setlist[itemize]{itemsep=0pt, topsep=0.4em, partopsep=0pt,itemindent=0.5\parindent}
\setlist[enumerate]{itemsep=0pt, topsep=0.4em, partopsep=0pt,itemindent=0.5\parindent}
\setlist[description]{itemsep=0pt, topsep=0.4em, partopsep=0pt,itemindent=0.5\parindent}

\pagestyle{fancy}
\fancyhf{}
\newcommand{\Lhead}[1]{\fancyhead[L]{#1}}
\newcommand{\Chead}[1]{\fancyhead[C]{#1}}
\newcommand{\Rhead}[1]{\fancyhead[R]{#1}}
\newcommand{\Lfoot}[1]{\fancyfoot[L]{#1}}
\newcommand{\Cfoot}[1]{\fancyfoot[C]{#1}}
\newcommand{\Rfoot}[1]{\fancyfoot[R]{#1}}
\renewcommand{\headrulewidth}{0.4pt}
\renewcommand{\footrulewidth}{0pt}