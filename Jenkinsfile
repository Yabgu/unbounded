pipeline {
  agent any

  stages {
    stage('Prepare') {
      steps {
        sh 'bash prepare.sh'
      }
    }

    stage('Build') {
      steps {
        sh 'cmake --build ./build'
      }
    }
    stage('Test') {
      steps {
        sh 'cmake --build ./build --target test'
      }
    }
  }
}
